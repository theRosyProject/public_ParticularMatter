# ParticularMatter

ParticularMatter (https://particularmatter.org/) provides air quality monitoring devices built around ESP32 (current platform) and ESP8266 (legacy). It measures particulate matter (PM2.5 / PM10) using a PMS5003 sensor, temperature, relative humidity, and atmospheric pressure using a BME280, CO2 using a SenseAir S8, and location using a NEO-6M GPS module. Readings are reported over MQTT and accessible via a built-in web portal.

This repository is the public release — it contains **firmware binaries**, **technical documentation**, and **presentation materials**. No source code is included (yet!).

---

## Repository structure

```
public_ParticularMatter/
├── builds/                          # Pre-built firmware binaries and flashing instructions
│   ├── esp32/                       # ESP32 firmware (current platform)
│   │   └── ParticularAir-Kongqi/    # ParticularAir-Kongqi device (v1.4.0)
│   └── esp8266/                     # ESP8266 firmware (legacy)
├── slides/                          # Presentation and dissemination materials
├── tech-doc/                        # Technical documentation
│   └── firmwares/
│       └── esp32/
│           └── ParticularAir-Kongqi/  # API docs, firmware modules, wiring, types
└── LICENSE                          # GPL v3
```

---

## Flashing the firmware

See [builds/README.md](builds/README.md) for firmware downloads and step-by-step flashing instructions for both ESP32 and ESP8266.

---

## Documentation

Technical documentation lives in [`tech-doc/`](tech-doc/). This includes firmware internals, REST API reference, data types, enums, sensor details, and wiring guides.

---

## License

GPL v3 — see [LICENSE](LICENSE).
