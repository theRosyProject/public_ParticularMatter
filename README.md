# ParticularMatter

ParticularMatter (https://particularmatter.org/) provides air quality monitoring devices built around ESP32 (current platform) and ESP8266 (legacy). It measures particulate matter (PM2.5 / PM10) using a PMS5003 sensor, temperature, relative humidity, and atmospheric pressure using a BME280, CO2 using a SenseAir S8, and location using a NEO-6M GPS module. Readings are reported over MQTT and accessible via a built-in web portal.

This repository is the public release — it contains **firmware binaries**, **technical documentation**, and **presentation materials**. No source code is included (yet!).

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

Device: **ParticularAir-Kongqi** — see [builds/esp32/ParticularAir-Kongqi/README.md](builds/esp32/ParticularAir-Kongqi/README.md) for full specs and flashing instructions.

Quick start:

```bash
pip install esptool
esptool --chip esp32 --port PORT --baud 921600 write-flash 0x1000 builds/esp32/ParticularAir-Kongqi/v1.4.0.bin
```

Replace `PORT` with your serial port (e.g. `/dev/tty.usbserial-0001` on macOS, `COM3` on Windows).

### ESP8266

Legacy firmware is available at `builds/esp8266/ESP8266-legacy.bin`. See [builds/esp8266/README.md](builds/esp8266/README.md) for flashing instructions and wiring.

```bash
pip install esptool
esptool --chip esp8266 --port PORT --baud 460800 write-flash 0x0 builds/esp8266/ESP8266-legacy.bin
```

---

## Documentation

Technical documentation lives in [`tech-doc/`](tech-doc/). This includes firmware internals, REST API reference, data types, enums, sensor details, and wiring guides.

---

## License

GPL v3 — see [LICENSE](LICENSE).
