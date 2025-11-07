/*
 ESP8266 Educational Firmware (SYNC skeleton)
 ------------------------------------------------------------
 What this is:
 • A teaching-oriented, privacy-safe version of a real firmware.
 • Demonstrates: Access Point (AP) + Captive Portal + Web form (EEPROM-backed),
 optional HTTPS registration (stubbed by default), periodic sensor read (PMS5003),
 and MQTT publish flow (stubbed by default).
 
 What this is NOT:
 • A drop‑in, production-ready firmware. Network calls and secrets are removed.
 • No real backend URL, credentials, or device identifiers are included.
 
 How to use this file:
 1) Read the inline documentation. Every major block explains WHY it exists
 and HOW you would adapt it.
 2) Search for the tag  [ADAPT]  to find places you need to change for your project.
 3) To keep this educational build safe-by-default, real networking is disabled.
 Flip the flag ENABLE_NETWORK to 1 only in your private fork.
 
 Safe-by-default build flags:
 - ENABLE_NETWORK = 0  → HTTPS registration and MQTT are mocked.
 - SHOW_SECRETS   = 0  → Web UI masks secret fields when logging.
 
 Boards: ESP8266 (Feather HUZZAH, NodeMCU, Wemos D1 mini...)
 Core:   ESP8266 Arduino Core 3.x (2.7+ works too)
 
 License: MIT — do whatever you want, no warranty.
 */

// =============================== Build Flags ===============================
#ifndef ENABLE_NETWORK
#define ENABLE_NETWORK 0   // 0 = stubbed registration & MQTT; 1 = real networking [ADAPT]
#endif
#ifndef SHOW_SECRETS
#define SHOW_SECRETS   0   // 0 = mask secrets in logs; 1 = reveal for debugging only
#endif

// =============================== Includes =================================
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#if ENABLE_NETWORK
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <PubSubClient.h>
#endif

// ============================ Generic Branding =============================
// All branding & URLs are deliberately generic.
// [ADAPT] Replace with your project/org when you restore networking.
static const char* kProjectName = "YourOrg Device Setup";

// ============================== AP Settings ================================
// This AP is only for first-time configuration via a captive portal.
// [ADAPT] You may randomize/append the last 3 bytes of the MAC to make SSIDs unique.
static const char* AP_SSID = "Device-Setup";     // safe, generic
static const char* AP_PASS = "setupsetup";       // demo only; consider open AP during setup

const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_GW(192, 168, 4, 1);
const IPAddress AP_MASK(255, 255, 255, 0);

// ============================== EEPROM Layout ==============================
// Keep it simple and well-documented. All strings are fixed-size to avoid
// dynamic allocation pitfalls and to make dumps readable.
constexpr size_t EEPROM_SIZE    = 2048;
constexpr uint32_t CONFIG_MAGIC = 0xEDUC0DE1;   // changed magic (privacy-safe)
constexpr size_t MAX_LEN        = 64;           // 63 + NUL
constexpr size_t UUID_LEN       = 37;           // 36 + NUL

struct ESPConfig {
    uint32_t magic;
    // User-entered fields (via captive portal form)
    char wifi_ssid[MAX_LEN];
    char wifi_pass[MAX_LEN];
    char user_email[MAX_LEN];
    char device_name[MAX_LEN];
    char one_time_key[MAX_LEN];   // registration code
    
    // Provisioning result (filled by registration API)
    char node_id[UUID_LEN];
    char mqtt_host[MAX_LEN];
    uint16_t mqtt_port;
    char mqtt_username[MAX_LEN];
    char mqtt_password[MAX_LEN];
    
    // Optional first sensor metadata
    char first_sensor_id[UUID_LEN];
    char first_sensor_sn[MAX_LEN];
    
    // Bookkeeping
    uint8_t registration_ok;      // 1 = success
};

ESPConfig config;  // single global config object

// ================================ Logging ==================================
// Minimal, timestamped log helpers that compile down to Serial.printf.
static inline void logf_(const char* lvl, const char* fmt, ...) {
    char buf[256];
    va_list ap; va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    Serial.printf("[+%10lu ms] [%s] %s\n", millis(), lvl, buf);
}
#define LOGI(...) logf_("INFO ", __VA_ARGS__)
#define LOGW(...) logf_("WARN ", __VA_ARGS__)
#define LOGE(...) logf_("ERROR", __VA_ARGS__)
#define LOGD(...) logf_("DEBUG", __VA_ARGS__)

// ================================ Servers ==================================
DNSServer dnsServer;               // captive DNS ("*" → AP_IP)
ESP8266WebServer server(80);       // tiny configuration UI

// =============================== PMS5003 ===================================
// We read PMS5003 frames using RX-only SoftwareSerial to save a UART.
// [ADAPT] Set PMS_RX to an input-capable pin on your board.
#define PMS_RX 13
SoftwareSerial pmsSerial; // configured in setup()

struct PMSData {
    uint16_t pm1_cf1  = 0;
    uint16_t pm25_cf1 = 0;
    uint16_t pm10_cf1 = 0;
    uint16_t pm1_atm  = 0;
    uint16_t pm25_atm = 0;
    uint16_t pm10_atm = 0;
    uint32_t ts_ms    = 0;
    bool     valid    = false;
};
PMSData g_pms;

// ================================ MQTT =====================================
#if ENABLE_NETWORK
WiFiClient mqttNet;
PubSubClient mqttClient(mqttNet);
uint32_t lastMqttConnAttempt = 0;
uint32_t mqttBackoffMs       = 0;
uint32_t lastMqttPub         = 0;
#endif

// ================================ Helpers ==================================
static bool haveWifiCreds() {
    return config.wifi_ssid[0] != '\0' && config.wifi_pass[0] != '\0';
}

static bool haveMqttCreds() {
    return config.registration_ok == 1 &&
        config.node_id[0] != '\0' &&
        config.mqtt_host[0] != '\0' &&
        config.mqtt_port != 0 &&
        config.mqtt_username[0] != '\0' &&
        config.mqtt_password[0] != '\0';
}

static void copyString(const String& src, char* dst, size_t dstSize) {
    size_t n = src.length();
    if (n >= dstSize) n = dstSize - 1;
    memcpy(dst, src.c_str(), n);
    dst[n] = '\0';
}

static String mask(const char* s, size_t keep = 2) {
    if (!s) return "";
    size_t n = strlen(s);
    if (n <= keep) return String(s);
    String out; out.reserve(n);
    for (size_t i = 0; i < n; ++i) out += (i < keep) ? s[i] : '*';
    return out;
}

static void dumpConfig(bool showSecrets) {
    LOGI("CONFIG dump (secrets %s):", showSecrets ? "VISIBLE" : "MASKED");
    LOGI("  SSID='%s'", config.wifi_ssid);
#if SHOW_SECRETS
    const bool reveal = showSecrets;
#else
    const bool reveal = false;
#endif
    LOGI("  PASS='%s'", reveal ? config.wifi_pass : mask(config.wifi_pass).c_str());
    LOGI("  USER='%s'", config.user_email);
    LOGI("  NAME='%s'", config.device_name);
    LOGI("  KEY ='%s'", reveal ? config.one_time_key : mask(config.one_time_key).c_str());
    LOGI("  node_id='%s'", config.node_id);
    LOGI("  mqtt_host='%s' port=%u", config.mqtt_host, config.mqtt_port);
    LOGI("  mqtt_user='%s'", config.mqtt_username);
    LOGI("  mqtt_pass='%s'", reveal ? config.mqtt_password : mask(config.mqtt_password).c_str());
    LOGI("  registration_ok=%u", config.registration_ok);
}

// ============================= Persistence =================================
static void loadConfig() {
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(0, config);
    if (config.magic != CONFIG_MAGIC) {
        LOGW("EEPROM magic mismatch. Re-init.");
        memset(&config, 0, sizeof(config));
        config.magic = CONFIG_MAGIC;
        EEPROM.put(0, config);
        EEPROM.commit();
    }
    dumpConfig(false);
}

static void saveConfig() {
    config.magic = CONFIG_MAGIC;
    EEPROM.put(0, config);
    if (EEPROM.commit()) LOGI("EEPROM commit OK.");
    else LOGE("EEPROM commit FAILED.");
}

static void clearConfig() {
    LOGW("Clearing full config...");
    memset(&config, 0, sizeof(config));
    EEPROM.put(0, config);
    if (EEPROM.commit()) LOGI("EEPROM cleared.");
    else LOGE("EEPROM clear commit FAILED.");
}

// ============================ Wi-Fi (AP + STA) =============================
static void setupAP() {
    LOGI("Bringing up AP '%s'...", AP_SSID);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(AP_IP, AP_GW, AP_MASK);
    bool ok = WiFi.softAP(AP_SSID, AP_PASS);
    delay(200);
    if (ok) LOGI("AP started on %s", WiFi.softAPIP().toString().c_str());
    else LOGE("AP start FAILED.");
    dnsServer.start(53, "*", AP_IP); // captive DNS
}

static bool connectSTA(uint32_t timeoutMs = 15000) {
    if (!haveWifiCreds()) { LOGW("STA connect skipped: empty SSID/PASS."); return false; }
    LOGI("Connecting STA to SSID '%s' (timeout %ums)...", config.wifi_ssid, timeoutMs);
    WiFi.mode(WIFI_AP_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);
    WiFi.begin(config.wifi_ssid, config.wifi_pass);
    
    uint32_t start = millis();
    wl_status_t st;
    do {
        delay(250);
        st = WiFi.status();
        Serial.print('.')
            ; } while (st != WL_CONNECTED && (millis() - start) < timeoutMs);
    Serial.println();
    
    if (st == WL_CONNECTED) {
        LOGI("STA connected. IP=%s, RSSI=%d", WiFi.localIP().toString().c_str(), WiFi.RSSI());
        return true;
    } else {
        LOGE("STA connect FAILED (status=%d).", (int)st);
        return false;
    }
}

static uint32_t lastStaAttempt = 0;
static uint32_t staBackoffMs   = 0;
static void ensureStaConnected() {
    wl_status_t st = WiFi.status();
    if (!haveWifiCreds() || st == WL_CONNECTED) return;
    uint32_t now = millis();
    if (now - lastStaAttempt < staBackoffMs) return;
    LOGI("STA ensure: not connected (status=%d). Attempting reconnect to '%s'...", (int)st, config.wifi_ssid);
    WiFi.mode(WIFI_AP_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);
    WiFi.begin(config.wifi_ssid, config.wifi_pass);
    
    lastStaAttempt = now;
    uint32_t next = staBackoffMs + 5000; // 5s,10s,...60s
    if (next > 60000) next = 60000;
    staBackoffMs = next;
}

// ============================= Registration =================================
// In this educational build, registration is STUBBED to return plausible values
// so you can exercise downstream logic without a live backend. Re-enable
// networking and replace the stub when moving to your private repo.

#if ENABLE_NETWORK
static String extractFirstJsonObject(const String& s) {
    int first = s.indexOf('{');
    int last  = s.lastIndexOf('}');
    if (first >= 0 && last > first) return s.substring(first, last + 1);
    return s;
}
#endif

static bool performRegistration() {
    if (String(config.one_time_key).length() == 0) { LOGW("Registration skipped: empty One Time Key."); return false; }
    
#if ENABLE_NETWORK
    // [ADAPT] Replace the entire block with your HTTPS POST using a pinned CA or fingerprint.
    if (!connectSTA()) { LOGE("Registration aborted: STA not connected."); return false; }
    LOGI("[NETWORK] Would POST registration payload and parse JSON here.");
    // Tip: Use BearSSL::WiFiClientSecure with a root CA and HTTPClient::begin(host, port, path, true)
    // Then deserialize with ArduinoJson into the fields below.
#else
    // ---------- STUB: pretend the backend replied with credentials ----------
    LOGI("[STUB] Simulating successful registration.");
    copyString("00000000-0000-0000-0000-000000000001", config.node_id, UUID_LEN);
    copyString("mqtt.example.local", config.mqtt_host, MAX_LEN);
    config.mqtt_port = 1883;
    copyString("demo-user", config.mqtt_username, MAX_LEN);
    copyString("demo-pass", config.mqtt_password, MAX_LEN);
    copyString("00000000-0000-0000-0000-00000000SENS", config.first_sensor_id, UUID_LEN);
    copyString("PMS5003-EDU", config.first_sensor_sn, MAX_LEN);
#endif
    config.registration_ok = 1;
    saveConfig();
    LOGI("Registration data stored.");
    dumpConfig(false);
    return true;
}

// ============================== PMS5003 I/O ================================
static bool pmsReadN(uint8_t* buf, size_t n, uint32_t timeoutMs = 200) {
    uint32_t start = millis(); size_t got = 0;
    while (got < n && (millis() - start) < timeoutMs) {
        while (pmsSerial.available() && got < n) buf[got++] = (uint8_t)pmsSerial.read();
        if (got < n) delay(2);
    }
    return got == n;
}

static bool readPMS5003Frame(PMSData& out) {
    // Find header 0x42, 0x4D
    uint32_t start = millis(); int state = 0;
    while ((millis() - start) < 200) {
        if (!pmsSerial.available()) { delay(2); continue; }
        int b = pmsSerial.read(); if (b < 0) continue;
        if (state == 0 && b == 0x42) state = 1;
        else if (state == 1 && b == 0x4D) { state = 2; break; }
        else state = 0;
    }
    if (state != 2) return false;
    
    uint8_t lenb[2]; if (!pmsReadN(lenb, 2)) return false;
    uint16_t frameLen = (uint16_t)lenb[0] << 8 | lenb[1];
    if (frameLen < 28 || frameLen > 64) return false;
    
    uint8_t data[64];
    if (frameLen > sizeof(data)) return false;
    if (!pmsReadN(data, frameLen)) return false;
    
    uint16_t sum = 0x42 + 0x4D + lenb[0] + lenb[1];
    for (size_t i = 0; i < (size_t)frameLen - 2; ++i) sum += data[i];
    uint16_t chk = ((uint16_t)data[frameLen - 2] << 8) | data[frameLen - 1];
    if (sum != chk) { LOGW("PMS checksum mismatch: calc=%u, frame=%u", sum, chk); return false; }
    
    auto wordAt = [&](int idx)->uint16_t { int i = idx * 2; return (uint16_t)data[i] << 8 | data[i+1]; };
    
    out.pm1_cf1  = wordAt(0);
    out.pm25_cf1 = wordAt(1);
    out.pm10_cf1 = wordAt(2);
    out.pm1_atm  = wordAt(3);
    out.pm25_atm = wordAt(4);
    out.pm10_atm = wordAt(5);
    out.ts_ms    = millis();
    out.valid    = true;
    return true;
}

static void pollPMS5003() {
    PMSData tmp;
    if (readPMS5003Frame(tmp)) {
        g_pms = tmp;
        LOGI("PMS ok: CF1[%u/%u/%u] ATM[%u/%u/%u] µg/m³",
             g_pms.pm1_cf1, g_pms.pm25_cf1, g_pms.pm10_cf1,
             g_pms.pm1_atm, g_pms.pm25_atm, g_pms.pm10_atm);
    }
}

// ============================== MQTT (stub) ================================
#if ENABLE_NETWORK
static String mqttTopic() {
    String t = "measurements/"; t += config.node_id; t += "/"; t += config.first_sensor_id; return t;
}

static String makeMeasurementPayload(float pm1, float pm25, float pm10) {
    char buf[128];
    snprintf(buf, sizeof(buf),
             "{\"measurement\":{\"pm1\":%.1f,\"pm25\":%.1f,\"pm10\":%.1f}}",
             pm1, pm25, pm10);
    return String(buf);
}

static void mqttEnsureConnected() {
    if (!haveMqttCreds()) return;
    if (mqttClient.connected()) return;
    uint32_t now = millis();
    if (now - lastMqttConnAttempt < mqttBackoffMs) return;
    mqttClient.setServer(config.mqtt_host, config.mqtt_port);
    LOGI("MQTT: connecting to %s:%u as '%s'...", config.mqtt_host, config.mqtt_port, config.node_id);
    bool ok = mqttClient.connect(config.node_id, config.mqtt_username, config.mqtt_password);
    if (ok) { LOGI("MQTT: connected."); mqttBackoffMs = 0; }
    else { LOGE("MQTT: connect failed (rc=%d).", mqttClient.state()); mqttBackoffMs = min<uint32_t>(mqttBackoffMs + 5000, 60000); }
    lastMqttConnAttempt = now;
}

static void mqttMaybePublish() {
    if (!haveMqttCreds() || !mqttClient.connected() || !g_pms.valid) return;
    static uint32_t lastMqttPub = 0; uint32_t now = millis();
    if (now - lastMqttPub < 20000) return; // ~20s
    lastMqttPub = now;
    String topic   = mqttTopic();
    String payload = makeMeasurementPayload(g_pms.pm1_atm, g_pms.pm25_atm, g_pms.pm10_atm);
    LOGI("MQTT PUB -> topic='%s' payload=%s", topic.c_str(), payload.c_str());
    if (!mqttClient.publish(topic.c_str(), payload.c_str(), true)) LOGE("MQTT publish failed (rc=%d).", mqttClient.state());
}
#else
static void mqttEnsureConnected() { /* stub: no-op in educational build */ }
static void mqttMaybePublish()   { /* stub: print instead of publish */
    if (!g_pms.valid || !config.registration_ok) return;
    LOGI("[STUB MQTT] Would publish ATM: pm1=%u pm25=%u pm10=%u",
         g_pms.pm1_atm, g_pms.pm25_atm, g_pms.pm10_atm);
}
#endif

// ============================== HTML & Pages ===============================
static String htmlHeader(const String& title) {
    String h;
    h += "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'>";
    h += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    h += "<title>" + title + "</title>";
    h += "<style>body{font-family:system-ui,-apple-system,Segoe UI,Roboto,Ubuntu,Noto Sans,Arial,sans-serif;max-width:800px;margin:24px auto;padding:0 16px}";
    h += "h1{font-size:1.6rem;margin:.2rem 0}.subtitle{margin:0 0 1rem;color:#444}";
    h += "h2{font-size:1.2rem;margin-top:1.2rem}label{display:block;margin:.6rem 0 .2rem}";
    h += "input[type=text],input[type=password],input[type=email]{width:100%;padding:.6rem;border:1px solid #ccc;border-radius:8px}";
    h += "button, input[type=submit]{padding:.6rem 1rem;border:0;border-radius:8px;margin-top:1rem;cursor:pointer}";
    h += "nav a{margin-right:1rem}footer{margin-top:2rem;color:#666;font-size:.9rem}";
    h += ".pm{border-radius:12px;padding:12px 16px;background:#f4f6fb;border:1px solid #e1e5f2;margin:8px 0 16px}";
    h += ".ok{color:#0a7a2f}.warn{color:#a66a00}.err{color:#b00020}";
    h += "code{background:#f6f8fa;padding:0 .25rem;border-radius:4px}";
    h += "</style></head><body>";
    h += "<header class='pm'><h1>" + String(kProjectName) + "</h1>";
    h += "<p class='subtitle'>This is an educational, non-production configuration portal.</p></header>";
    h += "<nav><a href='/'>&#x1F3E0; Home</a><a href='/clear'>Clear</a><a href='/reboot'>Reboot</a><a href='/status'>Status</a></nav>";
    return h;
}

static String htmlFooter() {
    String f;
    f += "<footer>Setup portal · AP " + WiFi.softAPIP().toString() + "</footer>";
    f += "</body></html>";
    return f;
}

static String renderFormPage() {
    String page = htmlHeader("Device Setup");
    page += "<h2>Configure Wi‑Fi & Registration</h2>";
    page += "<form method='POST' action='/save'>";
    page += "<label>Wi‑Fi SSID</label><input name='wifi_ssid' type='text' placeholder='MyHomeWiFi' value='" + String(config.wifi_ssid) + "' maxlength='" + String(MAX_LEN - 1) + "'>";
    page += "<label>Wi‑Fi password</label><input name='wifi_pass' type='password' placeholder='••••••••' value='" + String(config.wifi_pass) + "' maxlength='" + String(MAX_LEN - 1) + "'>";
    page += "<label>User Email</label><input name='user_email' type='email' placeholder='you@example.com' value='" + String(config.user_email) + "' maxlength='" + String(MAX_LEN - 1) + "'>";
    page += "<label>Device Name</label><input name='device_name' type='text' placeholder='Node‑Kitchen' value='" + String(config.device_name) + "' maxlength='" + String(MAX_LEN - 1) + "'>";
    page += "<label>One‑Time Key</label><input name='one_time_key' type='text' placeholder='Paste code' value='" + String(config.one_time_key) + "' maxlength='" + String(MAX_LEN - 1) + "'>";
    page += "<input type='submit' value='Save'>";
    page += "</form>";
    
    page += "<h2>Registration Status</h2>";
    if (config.registration_ok) {
        page += "<p class='ok'>Registered ✔</p><ul>";
        page += "<li>node_id: <code>" + String(config.node_id) + "</code></li>";
        page += "<li>mqtt_host: <code>" + String(config.mqtt_host) + "</code></li>";
        page += "<li>mqtt_port: <code>" + String(config.mqtt_port) + "</code></li>";
        page += "<li>mqtt_username: <code>" + String(config.mqtt_username) + "</code></li>";
        page += "</ul>";
    } else {
        page += "<p class='warn'>Not registered yet.</p>";
    }
    
    page += "<h2>PMS5003 (latest)</h2>";
    if (g_pms.valid) {
        page += "<ul>";
        page += "<li>CF=1: PM1=<code>" + String(g_pms.pm1_cf1) + "</code>, PM2.5=<code>" + String(g_pms.pm25_cf1) + "</code>, PM10=<code>" + String(g_pms.pm10_cf1) + "</code> µg/m³</li>";
        page += "<li>ATM : PM1=<code>" + String(g_pms.pm1_atm) + "</code>, PM2.5=<code>" + String(g_pms.pm25_atm) + "</code>, PM10=<code>" + String(g_pms.pm10_atm) + "</code> µg/m³</li>";
        page += "<li>Updated: <code>+" + String((uint32_t)(millis() - g_pms.ts_ms)) + " ms</code> ago</li>";
        page += "</ul>";
    } else {
        page += "<p class='warn'>No valid PMS frame yet (warming up or not connected).</p>";
    }
    
    page += htmlFooter();
    return page;
}

static String renderSavedPage(bool regOk, const String& regMsg) {
    String page = htmlHeader("Saved");
    page += "<h2>Saved!</h2><p>Your values have been stored in non‑volatile memory.</p>";
    page += "<h2>Registration</h2>";
    if (regOk) {
        page += "<p class='ok'>Registration successful ✔</p>";
    } else {
        page += "<p class='err'>Registration failed ✖</p><p><small>" + regMsg + "</small></p>";
    }
    page += "<p><a href='/'>Go back</a> or <a href='/reboot'>Reboot now</a>.</p>";
    page += htmlFooter();
    return page;
}

static String renderStatusPage() {
    String page = htmlHeader("Status");
    page += "<h2>Runtime Status</h2><ul>";
    page += "<li>AP IP: <code>" + WiFi.softAPIP().toString() + "</code></li>";
    page += "<li>STA status: <code>" + String((int)WiFi.status()) + "</code></li>";
    page += "<li>STA IP: <code>" + WiFi.localIP().toString() + "</code></li>";
    page += "<li>RSSI: <code>" + String(WiFi.RSSI()) + " dBm</code></li>";
    page += "<li>Free heap: <code>" + String(ESP.getFreeHeap()) + "</code></li>";
    page += "</ul>";
    page += "<h2>Registration</h2><ul>";
    page += "<li>registration_ok: <code>" + String(config.registration_ok) + "</code></li>";
    page += "<li>node_id: <code>" + String(config.node_id) + "</code></li>";
    page += "<li>mqtt_host: <code>" + String(config.mqtt_host) + "</code></li>";
    page += "<li>mqtt_port: <code>" + String(config.mqtt_port) + "</code></li>";
    page += "<li>mqtt_username: <code>" + String(config.mqtt_username) + "</code></li>";
    page += "</ul>";
    page += htmlFooter();
    return page;
}

// =============================== HTTP Routes ===============================
static void handleRoot()   { server.send(200, "text/html", renderFormPage()); }

static void handleSave() {
    if (server.method() != HTTP_POST) { server.send(405, "text/plain", "Method Not Allowed"); return; }
    if (server.hasArg("wifi_ssid"))    copyString(server.arg("wifi_ssid"),    config.wifi_ssid,   MAX_LEN);
    if (server.hasArg("wifi_pass"))    copyString(server.arg("wifi_pass"),    config.wifi_pass,   MAX_LEN);
    if (server.hasArg("user_email"))   copyString(server.arg("user_email"),   config.user_email,  MAX_LEN);
    if (server.hasArg("device_name"))  copyString(server.arg("device_name"),  config.device_name, MAX_LEN);
    if (server.hasArg("one_time_key")) copyString(server.arg("one_time_key"), config.one_time_key,MAX_LEN);
    
    // Reset registration-derived fields so the flow restarts cleanly
    config.registration_ok = 0;
    config.node_id[0] = '\0';
    config.mqtt_host[0] = '\0';
    config.mqtt_username[0] = '\0';
    config.mqtt_password[0] = '\0';
    config.mqtt_port = 0;
    config.first_sensor_id[0] = '\0';
    config.first_sensor_sn[0] = '\0';
    saveConfig();
    
    // Attempt registration right away (stubbed by default)
    lastStaAttempt = 0; staBackoffMs = 0; WiFi.disconnect();
    ensureStaConnected();
    bool regOk = performRegistration();
    String regMsg = regOk ? "OK" : "See serial logs for diagnostics.";
    server.send(200, "text/html", renderSavedPage(regOk, regMsg));
}

static void handleClear() {
    clearConfig(); loadConfig();
    String page = htmlHeader("Cleared");
    page += "<h2>Configuration cleared</h2><p>EEPROM config has been cleared.</p><p><a href='/'>Return home</a></p>";
    page += htmlFooter();
    server.send(200, "text/html", page);
}

static void handleReboot() {
    String page = htmlHeader("Rebooting");
    page += "<h2>Rebooting...</h2><p>The device will restart in a few seconds.</p>";
    page += htmlFooter();
    server.send(200, "text/html", page);
    delay(500);
    ESP.restart();
}

static void handleStatus() { server.send(200, "text/html", renderStatusPage()); }

static void handleNotFound() {
    if (server.hostHeader() != AP_IP.toString()) {
        server.sendHeader("Location", String("http://") + AP_IP.toString(), true);
        server.send(302, "text/plain", "");
    } else {
        server.send(404, "text/plain", "Not Found");
    }
}

static void handleCaptiveProbes() {
    server.on("/generate_204", HTTP_ANY, [](){ server.send(200, "text/html", "<html><body>Open portal: <a href='/' >Home</a></body></html>"); });
    server.on("/hotspot-detect.html", HTTP_ANY, [](){ server.send(200, "text/html", "<html><body><b>Success</b> — <a href='/' >Open portal</a></body></html>"); });
    server.on("/ncsi.txt", HTTP_ANY, [](){ server.send(200, "text/plain", "Microsoft NCSI"); });
}

static void setupWeb() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.on("/clear", HTTP_GET, handleClear);
    server.on("/reboot", HTTP_GET, handleReboot);
    server.on("/status", HTTP_GET, handleStatus);
    handleCaptiveProbes();
    server.onNotFound(handleNotFound);
    server.begin();
    LOGI("HTTP server started on http://%s", WiFi.softAPIP().toString().c_str());
}

// ================================ Arduino ==================================
static uint32_t lastHeartbeat = 0;

void setup() {
    Serial.begin(115200);
    delay(50);
    Serial.println();
    LOGI("Booting educational build (SYNC skeleton)...");
    LOGI("Build: " __DATE__ " " __TIME__ " | Core: ESP8266 Arduino | Free heap at boot: %u", ESP.getFreeHeap());
    
    loadConfig();
    setupAP();
    setupWeb();
    
    // PMS5003 UART (small buffer saves RAM)
    pmsSerial.begin(9600, SWSERIAL_8N1, PMS_RX, -1, false, 128);
    if (!pmsSerial) LOGE("PMS SoftwareSerial config invalid (pin unsupported?)");
    pinMode(PMS_RX, INPUT_PULLUP);
    pmsSerial.listen();
    LOGI("PMS5003 serial started on RX=%d @9600", PMS_RX);
    
    // WiFi auto (STA)
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);
    if (haveWifiCreds()) {
        LOGI("Boot: attempting STA join to '%s'...", config.wifi_ssid);
        connectSTA(8000);
        lastStaAttempt = 0; staBackoffMs = 0;
    } else {
        LOGW("Boot: no WiFi credentials saved, staying AP‑only.");
    }
    
#if ENABLE_NETWORK
    // MQTT client sizing if enabled
    LOGI("Networking ENABLED — ensure you configured CA pinning and private URLs.");
#endif
    
    dumpConfig(false);
}

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
    
    // PMS poll (non-blocking)
    pollPMS5003();
    
    // Keep STA connected
    ensureStaConnected();
    
    // MQTT keepalive + publish
    mqttEnsureConnected();
#if ENABLE_NETWORK
    extern PubSubClient mqttClient; // already declared above when enabled
    mqttClient.loop();
#endif
    mqttMaybePublish();
    
    // Heartbeat every ~5s with a concise summary
    uint32_t now = millis();
    if (now - lastHeartbeat >= 5000) {
        lastHeartbeat = now;
        if (g_pms.valid) {
            LOGI("HB: WiFi.status=%d AP=%s STA_IP=%s RSSI=%d Heap=%u | PMS CF1[%u/%u/%u] ATM[%u/%u/%u]",
                 (int)WiFi.status(),
                 WiFi.softAPIP().toString().c_str(),
                 WiFi.localIP().toString().c_str(),
                 WiFi.RSSI(),
                 ESP.getFreeHeap(),
                 g_pms.pm1_cf1, g_pms.pm25_cf1, g_pms.pm10_cf1,
                 g_pms.pm1_atm, g_pms.pm25_atm, g_pms.pm10_atm);
        } else {
            LOGI("HB: WiFi.status=%d AP=%s STA_IP=%s RSSI=%d Heap=%u | PMS waiting...",
                 (int)WiFi.status(),
                 WiFi.softAPIP().toString().c_str(),
                 WiFi.localIP().toString().c_str(),
                 WiFi.RSSI(),
                 ESP.getFreeHeap());
        }
    }
}

/*
 =============================== ADAPT NOTES ===============================
 1) Registration (private repo):
 - Define your backend endpoint and TLS root CA. Example pattern:
 std::unique_ptr<BearSSL::WiFiClientSecure> ssl(new BearSSL::WiFiClientSecure);
 ssl->setTrustAnchors(&your_ca_store);  // avoid setInsecure()
 HTTPClient http; http.begin(*ssl, host, port, path, /* https= */ true);
 http.addHeader("Content-Type", "application/json");
 http.POST("{\"registration_code\":\"...\"}");
 deserializeJson(...) into config fields; saveConfig();
 
 2) MQTT (private repo):
 - Set clean topic layout and retained payload policy.
 - Increase PubSubClient buffer if your payload grows.
 - Consider TLS for MQTT as well (WiFiClientSecure + certificate pinning).
 
 3) Security:
 - Never ship setInsecure() in production.
 - Never commit keys/tokens/URLs. Use build-time secrets or a private header.
 - Consider a setup window (AP auto-disables after N minutes / first success).
 
 4) Resilience:
 - Exponential backoff for STA & MQTT reconnects is shown here.
 - Consider a watchdog strategy if registration gets stuck.
 
 5) Memory:
 - SoftwareSerial uses small buffers here; adjust for noisy lines.
 - Prefer STA-only mode during TLS if RAM is tight.
 
 6) UX:
 - Keep the form minimal; validate inputs client-side if desired.
 - Add a QR code with the AP URL/IP if helpful.
 ============================================================================
 */
