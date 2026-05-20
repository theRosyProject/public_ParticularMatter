# Firmware Modules

All reusable logic is organized into independent modules under `lib/`. Each module is a self-contained C++ class that is instantiated in `src/main.cpp` and wired together there.

---

## common/

Shared headers used across all modules. Not a class — these are just type and macro definitions.

| File | Description |
| --- | --- |
| `ConnectionState.h` | Enum for Wi-Fi connection states (`CONNECTED`, `CONNECTING`, etc.) |
| `NetworkState.h` | Enum for overall network/MQTT states |
| `Measurement.h` | Shared struct for sensor measurement payloads |
| `API_RESPONSE.h` | Helper functions for serializing uniform HTTP responses |
| `HELPER.h` | General utility macros |
| `LOGGING.h` | `LOGI` / `LOGE` logging macros (wraps `Serial.printf`) |

---

## ESPConfig

Persistent configuration stored in ESP32 NVS (Non-Volatile Storage). Survives reboots and factory resets can be triggered explicitly.

**Responsibilities:**
- Read / write device settings (Wi-Fi SSID/password, AP credentials, MQTT credentials, node ID)
- Provide a `getData()` accessor used by other modules

---

## WiFiManager

Manages the ESP32 Wi-Fi connection in STA mode. Operates asynchronously — call `maintain()` each loop iteration.

**Responsibilities:**
- Connect to a saved SSID
- Track `ConnectionState` (transitions: `DISCONNECTED` → `CONNECTING` → `CONNECTED` / `WRONG_PASSWORD` / `NO_NETWORK_FOUND`)
- Trigger network scan

---

## WebPortal

HTTP web server (port 80) that serves the LittleFS-hosted frontend and exposes the RESTful API. Operates asynchronously — call `maintain()` each loop iteration.

**Responsibilities:**
- Serve static files from LittleFS
- Register and handle all `/api/*` routes
- Drive the node registration flow (HTTPS call to backend, update `ESPConfig`)

See [`docs/api-docs/README.md`](../api-docs/README.md) for the full API reference.

---

## MQTTPublisher

MQTT client that publishes sensor data to the ParticularMatter platform. Operates asynchronously — call `maintain()` each loop iteration.

**Responsibilities:**
- Connect to the MQTT broker using credentials from `ESPConfig` (with 5–60 s exponential reconnect backoff)
- Each cycle, aggregate the latest snapshot from every sensor driver into a single `MeasurementData`
- Publish the aggregate as JSON to `measurements/{node_id}/{sensor_id}` when at least one sensor is valid (per-sensor validity → `null` for failed sensors). See [`docs/notes/mqtt.md`](../notes/mqtt.md) for the full payload schema.
- Forward each published `MeasurementData` to `XTSDStorageDriver::log()` for local CSV logging

---

## PMS5003Driver

Driver for the Plantower PMS5003 particulate matter sensor. Communicates over UART (RX pin 13).

**Responsibilities:**
- Parse binary data frames from the sensor
- Provide PM1.0, PM2.5, PM10 readings
- Non-blocking polling via `poll()`

---

## SenseAirCO2

Driver for the Senseair S8 CO2 sensor. Communicates over I2C.

**Responsibilities:**
- Request and parse CO2 concentration readings
- Non-blocking polling via `poll()`

---

## BME280Driver

Driver for the Bosch BME280 environmental sensor. Communicates over I2C.

**Responsibilities:**
- Read temperature, relative humidity, and barometric pressure
- Wraps the Adafruit BME280 library

---

## NEO6MGPSDriver

Driver for the u-blox NEO-6M GPS module. Communicates over UART1 (RX pin 14, TX pin 27, 9600 baud).

> ⚠️ Do **not** use GPIO 12 — it is the MTDI strapping pin, and GPS TX idles HIGH which forces the ESP32 to boot at 1.8 V flash voltage and bricks uploads.

**Responsibilities:**
- Parse NMEA sentences with TinyGPSPlus
- Provide latitude, longitude, altitude, HDOP, satellite count, and fix status
- Provide an ISO-8601 UTC `datetime` string when both GPS date and time are valid (used by MQTT payload and SD log filenames)

---

## XTSDStorageDriver

Driver for MicroSD card storage over VSPI (CS pin 33). Writes a CSV row per measurement, with daily file rotation and hot-plug support.

**Responsibilities:**
- Mount the SD card (`SD.begin()` with CARD_NONE detection); reinitialise every 3 s when removed
- Rotate log files daily: filename is `/YYYY-MM-DD_log.csv`, derived from the GPS UTC `datetime` of the row being written
- Use `/unknown_log.csv` as a fallback until GPS provides a valid time fix
- Lazily reopen the file as needed; on rotation, sniff the existing first line and append a header marker if the schema doesn't match (FAT cannot prepend)
- Detect I/O failures (card pulled mid-write) → `SD.end()` and schedule a 3 s reinit retry, so the next measurement triggers a fresh open + header check
- Write per-sensor invalid fields as **empty** values (not zeros), keeping partial readings unambiguous

**CSV columns:**

```
datetime,ts_ms,pm1,pm25,pm10,co2,temperature,humidity,pressure,lat,lng,altitude,hdop,satellites,gps_valid
```
