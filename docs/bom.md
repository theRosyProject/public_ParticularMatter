# Bill of Materials (BoM)
for **public_ParticularMatter** ESP8266 Node Assembly  
_Last updated: November 2025_

This document lists the core hardware components required to assemble a fully functional node for the **Particular Matter** firmware (ESP8266 + PMS5003 + BME280).

---

## 🧩 Core Components

| # | Component | Description | Supplier / Link | Notes |
|---|------------|--------------|------------------|-------|
| 1 | **Adafruit Feather HUZZAH with ESP8266** | Wi-Fi–enabled microcontroller board based on ESP8266. Includes LiPo charger, USB interface, and breakout pins. | [Adafruit Product #2821](https://www.adafruit.com/product/2821) | Purchase **loose-header** version for custom enclosure assembly. |
| 2 | **Adafruit BME280 Temperature, Humidity, and Pressure Sensor** | High-accuracy I²C/SPI sensor for ambient monitoring (STEMMA QT compatible). | [Adafruit Product #2652](https://www.adafruit.com/product/2652) | Use via I²C (default) for easy wiring; connect to 3V and GND. |
| 3 | **PM2.5 Air Quality Sensor (PMS5003)** | Laser scattering particulate matter sensor for PM1.0, PM2.5, PM10 measurement. Includes adapter board for breadboard/prototyping. | [Adafruit Product #3686](https://www.adafruit.com/product/3686) | Connect via RX/TX (5 V logic safe with Feather HUZZAH). Warm-up ~30 s. |

---

## ⚡ Power & Connectivity Accessories

| # | Accessory | Description | Example / Supplier | Notes |
|---|------------|--------------|--------------------|-------|
| 4 | **USB Wall Charger (x2)** | 5 V / 2 A adaptive fast chargers for powering the Feather HUZZAH or charging LiPo batteries. | Example: “Caricabatterie USB da 2 Pezzi, Adattatore di Ricarica Rapido per Samsung Galaxy” (Amazon / generic) | Any high-quality 5 V 2 A USB adapter is suitable. |
| 5 | **Micro-USB Type A Male Jack PCB Mount Connector** | Male Micro-USB plug for integrating power or programming access into custom PCB housings. | Example: “Micro USB Tipo A Maschio Jack Pocket Connector Montaggio PBC” (generic electronics supplier) | Optional: use if integrating into custom PCB or enclosure. |

---

## 🧰 Optional / Support Materials

| # | Item | Purpose |
|---|------|----------|
| 6 | Breadboard + Jumper Wires | Prototyping interconnections for PMS5003 + BME280 |
| 7 | Enclosure (custom or 3D-printed) | Protection and airflow design for sensors |
| 8 | STEMMA QT / JST SH cables | Plug-and-play wiring for BME280 sensor |

Particular Matter enclosure: https://makerworld.com/en/models/1912756-particulair-in-1 

---

## 💡 Assembly Notes

- The Feather HUZZAH can power the PMS5003 from its **USB 5 V pin**.
- Use the **GPIO 13** for the PMS5003 TX channel.
- The **BME280** connects via **I²C** (`SDA → GPIO4`, `SCL → GPIO5` by default).  
- Keep PMS5003 airflow unobstructed; mount vertically if possible.  
- Ensure all grounds (GND) are common between components.  
- The full firmware binary (`firmware.bin`) can be uploaded using **ESPHome** after hardware assembly.

---

## 📦 Summary Table

| Category | Estimated Qty | Typical Cost (USD) |
|-----------|----------------|--------------------|
| Core Components | 3 | ~$60–70 |
| Power Accessories | 2 | ~$15 |
| Connectors & Misc | 2–3 | ~$5 |
| **Total** | **~$80–90** | *(per node, excluding enclosure)* |

---

**Maintainer:** Particular Matter  
For educational and open hardware use.  
Licensed under [MIT](./LICENSE).