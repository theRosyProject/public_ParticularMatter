# Wiring Instructions

## Components

| Component | Description |
|-----------|-------------|
| **Adafruit Feather HUZZAH with ESP8266** | Main microcontroller board |
| **PMS5003** | PM2.5 Air Quality Sensor and Breadboard Adapter Kit |
| **Adafruit BME280** | I2C/SPI Temperature, Humidity, and Pressure Sensor (STEMMA QT) |
| **CO2 sensor (I2C)** | CO2 sensor with I2C interface |

---

## Wiring

### Power

All sensors share the same power rails from the Feather HUZZAH:

| Feather HUZZAH pin | Connects to |
|--------------------|-------------|
| `USB` (5V) | VCC of PMS5003, BME280, CO2 sensor |
| `GND` | GND of PMS5003, BME280, CO2 sensor |

> The `USB` pin on the Feather HUZZAH provides 5V when the board is powered via USB. The BME280 and CO2 sensor tolerate 3.3V–5V input; refer to their datasheets if powering from 3.3V instead.

---

### PMS5003 – PM2.5 Particle Sensor (UART)

Communication is one-way: only the TX line of the PMS5003 is read by the ESP8266.

| PMS5003 pin | Feather HUZZAH pin |
|-------------|-------------------|
| `VCC` | `USB` (5V) |
| `GND` | `GND` |
| `TX` | `GPIO13` |

> `GPIO13` maps to the label **`MOSI`** on the Feather HUZZAH silkscreen.
> In firmware, configure a `SoftwareSerial` (or hardware Serial) listener on GPIO13.

---

### BME280 – Temperature / Humidity / Pressure Sensor (I2C)

| BME280 pin | Feather HUZZAH pin |
|------------|-------------------|
| `VIN` | `USB` (5V) |
| `GND` | `GND` |
| `SDA` | `SDA` (GPIO4) |
| `SCL` | `SCL` (GPIO5) |

---

### CO2 Sensor (I2C)

Shares the I2C bus with the BME280.

| CO2 sensor pin | Feather HUZZAH pin |
|----------------|-------------------|
| `VCC` | `USB` (5V) |
| `GND` | `GND` |
| `SDA` | `SDA` (GPIO4) |
| `SCL` | `SCL` (GPIO5) |

> Both the BME280 and the CO2 sensor sit on the same I2C bus (SDA/SCL). Ensure their I2C addresses do not conflict; if needed, change the address using the address-select pin available on some modules.

---

## Summary Diagram

```
Feather HUZZAH ESP8266
┌──────────────────────────┐
│                          │
│  USB (5V) ───────────────┼──── VCC (PMS5003)
│                     ─────┼──── VIN (BME280)
│                     ─────┼──── VCC (CO2 sensor)
│                          │
│  GND ────────────────────┼──── GND (PMS5003)
│                     ─────┼──── GND (BME280)
│                     ─────┼──── GND (CO2 sensor)
│                          │
│  GPIO13 (MOSI) ──────────┼──── TX  (PMS5003)
│                          │
│  SDA (GPIO4) ────────────┼──── SDA (BME280)
│                     ─────┼──── SDA (CO2 sensor)
│                          │
│  SCL (GPIO5) ────────────┼──── SCL (BME280)
│                     ─────┼──── SCL (CO2 sensor)
│                          │
└──────────────────────────┘
```
