# ParticularAir-Kongqi — v1.4.0

> **ParticularAir-Kongqi** — Real-time air quality monitoring. Local storage. Always accessible.

---

<!-- cover image -->
<!-- ![ParticularAir-Kongqi device](./assets/device-cover.jpg) -->

---

## What is this?

ParticularAir-Kongqi is an open-source air quality monitoring device built around an ESP32. It measures PM2.5, CO₂, temperature, humidity, and atmospheric pressure, while logging GPS coordinates for every reading. All data is stored locally on an SD card and can be synced to a cloud platform over MQTT.

No app required — open a browser on your phone and you can view live readings, download history, or adjust device settings.

---

## What's in v1.4.0

### 1. Five sensors, one device

| Parameter | Sensor | Output |
| --- | --- | --- |
| Particulate matter | PMS5003 | PM1.0 / PM2.5 / PM10 (μg/m³) |
| Carbon dioxide | SenseAir S8 | CO₂ (ppm) |
| Temperature | BME280 | °C |
| Relative humidity | BME280 | % RH |
| Atmospheric pressure | BME280 | hPa |
| GPS | NEO-6M | Lat / Lon / Altitude / UTC |

> Every reading is timestamped and geotagged — well suited for mobile monitoring and environmental surveys.

---

<!-- sensor wiring diagram -->
<!-- ![Sensor wiring diagram](./assets/wiring-diagram.jpg) -->

---

### 2. No app — the browser is the interface

The device hosts a built-in Web Portal: a Vue 3 frontend bundled into the ESP32's LittleFS filesystem and served over HTTP.

- Connect to the device's Wi-Fi hotspot, open a browser, and go to `192.168.4.1`
- Works on phones, tablets, and desktops — responsive layout
- Fully usable offline

<!-- video: Web Portal walkthrough -->
<!--
<video controls width="100%">
  <source src="./assets/web-portal-demo.mp4" type="video/mp4">
  Your browser does not support video playback.
</video>
-->

<!-- or embed YouTube -->
<!-- [![Web Portal Demo](https://img.youtube.com/vi/YOUR_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID) -->

---

### 3. Dual Wi-Fi — always reachable

The device runs **AP mode** (hotspot) and **STA mode** (home Wi-Fi) simultaneously.

- **AP always on**: the hotspot stays up regardless of home Wi-Fi status — the Web Portal is always accessible
- **STA auto-reconnect**: exponential backoff retry on disconnect; cloud sync resumes automatically when the connection is restored
- First-time setup is guided through a Captive Portal — supported on iOS, Android, and Windows

```
Your phone
    │
    │  connect to ESP32 hotspot
    ▼
192.168.4.1 ──► Web Portal (configure Wi-Fi)
                    │
                    │  save → device reboots, connects to home router
                    ▼
              Home Wi-Fi ──► MQTT cloud platform
```

---

<!-- Wi-Fi setup flow screenshot -->
<!-- ![Wi-Fi setup flow](./assets/wifi-setup-flow.png) -->

---

### 4. Full local storage on SD card

All measurements are written to a MicroSD card as daily CSV files, with every sensor value, GPS coordinate, and timestamp included.

- Filename format: `YYYY-MM-DD_log.csv`
- Data is never lost when offline
- Single-day or full export available directly from the Web Portal

```
/measurements/
├── 2026-05-18_log.csv
├── 2026-05-19_log.csv
└── 2026-05-20_log.csv
```

System logs are written to `/log/system.log` with automatic rotation and are viewable live from the Web Portal.

---

### 5. Real-time MQTT cloud sync

After backend pairing is complete, the device publishes aggregated sensor data as JSON every 10 seconds:

```
measurements/{node_id}/{sensor_id}
```

- Separate topics for PM, CO₂, environment, and GPS
- If a sensor fails, that field returns `null` — other readings are unaffected
- Local storage continues uninterrupted during disconnection; sync resumes on reconnect

---

## Quick start

### Step 1 — First power-on

On boot, the device broadcasts a Wi-Fi hotspot named `ESP32_Setup` (default password: `12345678`).

<!-- ![First power-on hotspot](./assets/step1-ap.png) -->

### Step 2 — Connect to the Captive Portal

Connect your phone or computer to the hotspot. The setup page should appear automatically. If it doesn't, open a browser and go to `192.168.4.1`.

<!-- ![Captive Portal setup page](./assets/step2-portal.png) -->

### Step 3 — Enter your home Wi-Fi

Select your Wi-Fi network, enter the password, and save. The device will reboot, connect to your home network, and keep the hotspot running for future access.

<!-- ![Wi-Fi credentials setup](./assets/step3-wifi.png) -->

### Step 4 — Pair with the backend

Go to the **Node settings** page, enter the one-time key provided by the backend, and complete pairing. Once paired, the device receives its Node ID and begins MQTT publishing.

<!-- ![Node pairing flow](./assets/step4-pairing.png) -->

---

## Specifications

| | |
| --- | --- |
| MCU | ESP32 (Adafruit Feather HUZZAH32) |
| Recommended flash | 4 MB |
| OS | FreeRTOS (via Arduino / PlatformIO) |
| Frontend | Vue 3 + Vite + Pinia |
| Protocols | Wi-Fi 802.11 b/g/n, HTTP, MQTT, HTTPS |
| Storage | MicroSD (SPI), LittleFS (internal flash) |
| Firmware version | v1.4.0 |
| License | See LICENSE |

---

## Known limitations

- OTA updates are not supported — firmware must be flashed via USB
- MQTT is TCP only; TLS is not yet supported
- SD card hot-swap has a ~3 second detection delay
- Backend HTTPS uses `setInsecure()` — certificate validation is skipped (suitable for test environments)

---

## Further reading

- [API reference](../../../tech-doc/firmwares/esp32/ParticularAir-Kongqi/dev-doc/api-docs/README.md)
- [Firmware modules](../../../tech-doc/firmwares/esp32/ParticularAir-Kongqi/dev-doc/firmware/modules.md)
- [Sensor wiring](../../../tech-doc/firmwares/esp32/ParticularAir-Kongqi/dev-doc/firmware/sensors.md)
- [MQTT payload format](../../../tech-doc/firmwares/esp32/ParticularAir-Kongqi/dev-doc/notes/mqtt.md)

