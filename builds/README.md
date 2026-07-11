# Builds

This folder contains pre-built firmware binaries for ParticulAir, along with flashing instructions.

Find your device in the table below, download the `.bin` file from the corresponding folder, and follow the installation guide.

## Firmware

| Device | Platform | Latest | Folder |
|--------|----------|--------|--------|
| ParticularAir-1 | ESP8266 | — | — |
| ParticularAir-Kongqi | ESP32 | [v1.4.0](esp32/ParticularAir-Kongqi/v1.4.0.bin) | [esp32/ParticularAir-Kongqi/](esp32/ParticularAir-Kongqi/) |

---

## Installation

After downloading the `.bin` file, choose one of the methods below.

### Method 1 — ESPHome Web (no command line)

Recommended for most users. Requires Chrome or Edge.

1. Download the `.bin` file for your device from the table above.
2. Open [https://web.esphome.io/](https://web.esphome.io/).
3. Click **Connect** and select your device's serial port from the popup.
4. Once connected, click **Install** → **Browse** and select the downloaded `.bin` file.
5. Confirm and wait for the process to complete. The device will reboot automatically.

### Method 2 — Command line (esptool)

#### Install esptool

```bash
pip install esptool
```

#### Find your serial port

- macOS / Linux: `ls /dev/tty.usb*`
- Windows: Device Manager → Ports (COM & LPT)

#### Flash

Replace `PORT` with your actual serial port (e.g. `/dev/tty.usbserial-0001` on macOS, `COM3` on Windows):

```bash
esptool --chip esp32 --port PORT --baud 921600 write-flash 0x1000 v1.4.0.bin
```

> If flashing fails, try a lower baud rate: `--baud 115200`
