# Bill of Materials (BoM)
for **public_ParticularMatter** ESP8266 Node Assembly  
_Last updated: November 2025_

This document lists the core hardware components required to assemble a fully functional node for the **Particular Matter** firmware (ESP8266 + PMS5003 + BME280 + Senseair Sunrise CO2).

---

## 🧩 Core Components

| # | Component | Description | Supplier / Link | Unit Price (USD) | Unit Price (EUR) | Notes |
|---|------------|--------------|------------------|-----------------|-----------------|-------|
| 1 | **Adafruit Feather HUZZAH with ESP8266** | Wi-Fi–enabled microcontroller board based on ESP8266. Includes LiPo charger, USB interface, and breakout pins. | [Adafruit Product #2821](https://www.adafruit.com/product/2821) | $14.95 | €12.86 | Purchase **loose-header** version for custom enclosure assembly. |
| 2 | **Adafruit BME280 Temperature, Humidity, and Pressure Sensor** | High-accuracy I²C/SPI sensor for ambient monitoring (STEMMA QT compatible). Includes on-board 10 kΩ I²C pull-up resistors. | [Adafruit Product #2652](https://www.adafruit.com/product/2652) | $14.95 | €12.86 | Use via I²C (default) for easy wiring; connect to 3V and GND. |
| 3 | **PM2.5 Air Quality Sensor (PMS5003)** | Laser scattering particulate matter sensor for PM1.0, PM2.5, PM10 measurement. Includes adapter board for breadboard/prototyping. | [Adafruit Product #3686](https://www.adafruit.com/product/3686) | $39.95 | €34.36 | Connect via RX/TX (5 V logic safe with Feather HUZZAH). Warm-up ~30 s. |
| 4 | **Senseair Sunrise CO2 Sensor (006-0-0008)** | NDIR CO2 sensor, 0–5000 ppm range, I²C interface, ultra-low power consumption. | [Senseair](https://senseair.com/products/miniature-oem/sunrise/) | — | — | **Provided free of charge by Senseair**, project partner. Not available for individual purchase through standard channels. |

---

## ⚡ Power & Connectivity Accessories

| # | Accessory | Description | Example / Supplier | Notes |
|---|------------|--------------|--------------------|-------|
| 4 | **USB Wall Charger (x2)** | 5 V / 2 A adaptive fast chargers for powering the Feather HUZZAH or charging LiPo batteries. | Example: “Caricabatterie USB da 2 Pezzi, Adattatore di Ricarica Rapido per Samsung Galaxy” (Amazon / generic) | **Not** any high-quality 5 V 2 A USB adapter is suitable. |
| 5 | **Micro-USB Type A Male Jack PCB Mount Connector** | Male Micro-USB plug for integrating power or programming access into custom PCB housings. | Example: “Micro USB Tipo A Maschio Jack Pocket Connector Montaggio PBC” (generic electronics supplier) | Needed to plug the device on wall socket. |

---

## 🧰 Optional / Support Materials

| # | Item | Purpose |
|---|------|----------|
| 6 | Breadboard + Jumper Wires | Prototyping interconnections for PMS5003 + BME280 |
| 7 | Enclosure (custom or 3D-printed) | Protection and airflow design for sensors |
| 8 | STEMMA QT / JST SH cables | Plug-and-play wiring for BME280 sensor |

- **3D printable enclosure files** are in [`src/3D_printer/`](../src/3D_printer/) — also published on MakerWorld: https://makerworld.com/en/models/1912756-particulair-in-1
- **Wiring instructions** (pin connections, pull-up notes, Senseair Sunrise setup) are in [`src/wiring/wiring_instructions.md`](../src/wiring/wiring_instructions.md)

---

## 💡 Assembly Notes

- The Feather HUZZAH can power all sensors from its **USB 5 V pin** (VCC/VIN/VBB).
- Use **GPIO13** for the PMS5003 TX channel (one-way, no TX wire needed from ESP8266).
- The **BME280** and **Senseair Sunrise** both connect via **I²C** (`SDA → GPIO4`, `SCL → GPIO5`).
- The BME280 STEMMA QT breakout's on-board 10 kΩ pull-up resistors serve the entire I²C bus — no additional pull-up resistors are required.
- The Senseair Sunrise **VDDIO** pin must be connected to the **3.3 V** rail (not 5 V), and **COMSEL** must be tied to **GND** to select I²C mode.
- Keep PMS5003 airflow unobstructed; mount vertically if possible.
- Ensure all grounds (GND) are common between components.
- The full firmware binary (`firmware.bin`) can be uploaded using **ESPHome** after hardware assembly.

---

## 📦 Summary Table

| Category | Qty | Unit Cost (USD) | Unit Cost (EUR) |
|-----------|-----|-----------------|-----------------|
| Adafruit Feather HUZZAH with ESP8266 | 1 | $14.95 | €12.86 |
| Adafruit BME280 STEMMA QT | 1 | $14.95 | €12.86 |
| PM2.5 Air Quality Sensor (PMS5003) | 1 | $39.95 | €34.36 |
| Senseair Sunrise CO2 sensor | 1 | — *(provided by Senseair)* | — |
| Power Accessories | 2 | ~$15 | ~€13 |
| Connectors & Misc | 2–3 | ~$5 | ~€4.50 |
| **Total (purchasable components)** | | **~$90** | **~€77** |

> Prices as listed on Adafruit at time of last update. Excludes enclosure and shipping.

---

**Maintainer:** Particular Matter  
For educational and open hardware use.  
Licensed under [MIT](./LICENSE).