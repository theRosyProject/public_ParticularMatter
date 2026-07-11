# Wiring Instructions

## Components

| Component | Description |
|-----------|-------------|
| **Adafruit Feather HUZZAH with ESP8266** | Main microcontroller board |
| **PMS5003** | PM2.5 Air Quality Sensor and Breadboard Adapter Kit |
| **Adafruit BME280** | I2C/SPI Temperature, Humidity, and Pressure Sensor (STEMMA QT) |
| **Senseair Sunrise CO2 sensor** | CO2 sensor (I2C), Senseair Sunrise series (TDE7318 / TDE5531) |

---

## Wiring

### Power

| Feather HUZZAH pin | Connects to |
|--------------------|-------------|
| `USB` (5V) | `VCC` of PMS5003, `VIN` of BME280, `VBB` of Senseair Sunrise |
| `3V` (3.3V) | `VDDIO` of Senseair Sunrise |
| `GND` | `GND` of all sensors |

> The `USB` pin provides 5V when powered via USB. `VDDIO` on the Senseair Sunrise must be supplied at the I/O logic voltage of the host; use `3V` (3.3V) since all ESP8266 GPIOs operate at 3.3V.

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

> The **Adafruit BME280 STEMMA QT breakout includes 10 kΩ pull-up resistors on SDA and SCL**. These serve the entire I²C bus, including the Senseair Sunrise. No additional external pull-up resistors are required when using this specific breakout board.

---

### Senseair Sunrise – CO2 Sensor (I2C)

The Sunrise has additional control pins beyond the basic I2C lines. All must be connected correctly.

| Senseair Sunrise pin | Feather HUZZAH pin | Notes |
|----------------------|--------------------|-------|
| `VBB` | `USB` (5V) | Main power supply |
| `VDDIO` | `3V` (3.3V) | I/O logic voltage — must match ESP8266 GPIO level |
| `GND` | `GND` | |
| `EN` | `USB` (5V) via 10kΩ pull-up | Drive HIGH for continuous operation; control via GPIO for low-power mode |
| `COMSEL` | `GND` | Selects I2C mode (leave floating or connect to GND) |
| `RXD/SDA` | `SDA` (GPIO4) | I2C data |
| `TXD/SCL` | `SCL` (GPIO5) | I2C clock |
| `nRDY` | optional GPIO | Active-low ready signal; can be left unconnected if using timing-based polling |

> **I2C address**: default is `0x68`. No conflict with the BME280 (default `0x76`/`0x77`).
>
> **Wake-up**: the Sunrise spends most of its time in deep sleep. Before any I2C read/write, the host must send the sensor's address to wake it up (START + address + STOP). The sensor will NACK this first byte if asleep, then ACK subsequent communication within 15 ms.

---

### I2C Pull-up Resistors

The Senseair Sunrise (TDE5531) includes only a 100 kΩ internal pull-up on SCL and **no internal pull-up on SDA**. The I²C specification and TDE5531 require the SDA line to be pulled up externally. TDE5531 recommends a pull-up range of **5 kΩ–15 kΩ** on both lines for reliable operation at 100 kbit/s (standard mode).

The ESP8266's internal pull-ups (configurable via software) are **not sufficient**: they are in the range of 30 kΩ–100 kΩ, far above the 15 kΩ maximum recommended by the Senseair datasheet. Using them would result in slow signal rise times and likely communication failures with the Sunrise.

**When using the Adafruit BME280 STEMMA QT breakout, no additional external resistors are needed.** The Adafruit BME280 breakout includes **10 kΩ pull-up resistors on both SDA and SCL** as part of its on-board circuitry. Since all I²C devices share the same bus lines, these pull-ups apply to the Sunrise as well. 10 kΩ falls within the 5 kΩ–15 kΩ recommended range, satisfying the Senseair requirement.

If the BME280 breakout is replaced with a bare chip or a different module that does not include on-board pull-ups, add external 10 kΩ resistors:

| Line | Pull-up resistor | Connected between |
|------|-----------------|-------------------|
| `SDA` (GPIO4) | 10 kΩ | `SDA` and `3V` (3.3V) |
| `SCL` (GPIO5) | 10 kΩ | `SCL` and `3V` (3.3V) |

---

## Summary Diagram

```
Feather HUZZAH ESP8266
┌──────────────────────────┐
│                          │
│  USB (5V) ───────────────┼──── VCC  (PMS5003)
│                     ─────┼──── VIN  (BME280)
│                     ─────┼──── VBB  (Senseair Sunrise)
│                     ─────┼──── EN   (Senseair Sunrise, via 10kΩ to VBB)
│                          │
│  3V (3.3V) ──────────────┼──── VDDIO (Senseair Sunrise)
│                          │
│  GND ────────────────────┼──── GND    (PMS5003)
│                     ─────┼──── GND    (BME280)
│                     ─────┼──── GND    (Senseair Sunrise)
│                     ─────┼──── COMSEL (Senseair Sunrise) → selects I2C
│                          │
│  GPIO13 (MOSI) ──────────┼──── TX     (PMS5003)
│                          │
│  SDA (GPIO4) ────────────┼──── SDA    (BME280)
│                     ─────┼──── RXD/SDA (Senseair Sunrise)
│                          │
│  SCL (GPIO5) ────────────┼──── SCL    (BME280)
│                     ─────┼──── TXD/SCL (Senseair Sunrise)
│                          │
└──────────────────────────┘
```
