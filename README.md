# ParticularMatter

ParticularMatter (https://particularmatter.org/) provides air quality monitoring devices built around ESP8266 and ESP32. It measures particulate matter (PM2.5 / PM10) using a PMS5003 sensor, air temperature, relative humidity and atmospheric pressure (T/RH/P) using the Bosch BME280, carbon dioxide (CO2) using the Senseair Sunrise sensor, and reports readings over MQTT.

This repository is the public release — it contains **firmware binaries**, **technical documentation**, and **presentation materials**. No source code is included (yet!).

**3D model (enclosure)** for the EPS8266 device mouting just the PMS5003 sensor:  [ParticulAIR-In 1 on MakerWorld](https://makerworld.com/en/models/1912756-particulair-in-1)

--> Models for the updated ESP32 version are under development.

---

## Repository structure

```
public_ParticularMatter/
├── builds/          # Pre-built firmware binaries and flashing instructions
│   ├── esp32/       # ESP32 firmware (current platform)
│   └── esp8266/     # ESP8266 firmware (legacy platform)
├── slides/          # Presentation and dissemination materials
├── tech-doc/        # Technical documentation (API, firmware internals, types, wiring)
└── LICENSE          # GPL v3
```

---

## Flashing the firmware

### ESP32

Pre-built binaries are in `builds/esp32/`. See [builds/esp32/README.md](builds/esp32/README.md) for full flashing instructions.

Quick start:

```bash
pip install esptool
esptool --chip esp32 --port PORT --baud 921600 write-flash 0x1000 builds/esp32/v1.4.0.bin
```

Replace `PORT` with your serial port (e.g. `/dev/tty.usbserial-0001` on macOS, `COM3` on Windows).

### ESP8266

... add, for compatibility with the legacy device and firmware.

---

## Documentation

Technical documentation lives in [`tech-doc/`](tech-doc/). This includes firmware internals, REST API reference, data types, enums, sensor details, and wiring guides.

---

## License

GPL v3 — see [LICENSE](LICENSE).
