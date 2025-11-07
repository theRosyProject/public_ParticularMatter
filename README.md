public_ParticularMatter

A public, educational release of the Particular Matter ESP8266 firmware. This repository demonstrates the structure and build process of the firmware while keeping all private backend and credential logic hidden.

The project includes both:
	•	a fully functional compiled firmware (firmware.bin) ready to be installed on an ESP8266 device using ESPHome, and
	•	an educational, non-functional C++ source (ParticularMatter_public.cpp) that documents the firmware architecture, logic flow, and key implementation concepts.

⸻

📂 Repository structure

public_ParticularMatter/
├── LICENSE                      # License information (MIT recommended)
├── README.md                    # Main documentation (this file)
├── public_ParticularMatter.Rproj # Project metadata
└── src/
    ├── build/                   # Working build artifacts and compiled firmware
    │   └── esp8266/
    │       ├── firmware.bin     # ✅ Working firmware (use with ESPHome)
    │       ├── firmware.elf     # Compiled ELF (debug info)
    │       └── ...              # Intermediate build objects and libraries
    └── cpp/
        ├── ParticularMatter_public.cpp  # 🧩 Educational, non-functional firmware skeleton
        └── README.md                    # Notes specific to the C++ source


⸻

🚀 Installation with ESPHome

The firmware.bin file inside src/build/esp8266/ is a ready-to-use firmware image compiled for ESP8266 boards (e.g., Feather HUZZAH, NodeMCU, Wemos D1 Mini).

Requirements
	•	ESPHome￼ (CLI or Home Assistant integration)
	•	An ESP8266 device
	•	A USB cable for flashing

Flash using ESPHome CLI
	1.	Connect your ESP8266 board to your computer.
	2.	Run the following command:

esphome run src/build/esp8266/firmware.bin

or, to upload manually:

esphome upload src/build/esp8266/firmware.bin

You can also flash via the ESPHome dashboard in Home Assistant.

After installation, the device will boot and start the firmware’s normal operation — setting up an access point, hosting a configuration page, and reading PMS5003 data.

⸻

🧠 About the educational source (ParticularMatter_public.cpp)

The file src/cpp/ParticularMatter_public.cpp is a redacted, non-functional version of the actual firmware used internally by Particular Matter. It is included for transparency and educational purposes.

It demonstrates how the full firmware:
	•	Brings up an Access Point + Captive Portal for first-time setup
	•	Hosts a Web UI to collect Wi-Fi credentials and device info
	•	Stores configuration safely in EEPROM
	•	Reads PMS5003 particulate matter sensor data via SoftwareSerial
	•	Performs device registration and MQTT publishing (stubbed in this public version)
	•	Implements robust logging and memory management for ESP8266 devices

This version is ideal for:
	•	Understanding the firmware design
	•	Learning about Wi-Fi AP setup, EEPROM storage, JSON parsing, and MQTT flow
	•	Adapting the structure for your own projects

⚠️ The educational firmware does not connect to any backend or broker. All sensitive constants, URLs, and tokens have been removed.

⸻

🔧 Adapting for private or research use

If you intend to rebuild the firmware using the educational source:
	1.	Define your own backend and MQTT credentials in a private header (e.g., backend_config.h).
	2.	Replace the stubbed HTTPS registration and MQTT routines with your own logic.
	3.	Recompile using PlatformIO or Arduino CLI with ESP8266 Core ≥ 3.x.

⸻

⚙️ Build Environment

The project is designed for:
	•	Board: ESP8266 (Feather HUZZAH / NodeMCU / Wemos D1 Mini)
	•	Core: ESP8266 Arduino Core 3.x (compatible with 2.7+)
	•	Dependencies:
	•	ESP8266WiFi
	•	ESP8266WebServer
	•	DNSServer
	•	EEPROM
	•	ArduinoJson
	•	SoftwareSerial
	•	(optional) ESP8266HTTPClient, PubSubClient

⸻

🔐 Security Notes
	•	This public release does not contain any private API endpoints or certificates.
	•	The functional firmware (firmware.bin) uses generic placeholder behavior suitable for demonstration.
	•	Always protect production firmware with proper TLS and credential storage.

⸻

🧰 Recommended .gitignore

If you fork or rebuild this project, include a .gitignore such as:

src/build/
*.bin
*.elf
.pio/
.vscode/
.DS_Store


⸻

🧩 License

This repository is provided under the MIT License (see LICENSE).

⸻

Maintainer: Particular Matter￼

For questions or collaborations, please contact the maintainers or open an issue in your fork.