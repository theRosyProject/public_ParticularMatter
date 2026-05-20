# ESP32 Firmware — v1.4.0

Pre-built firmware for ESP32 device.

## Flashing Instructions - No command line

### ESPHome Web 

*add guidelines* for non expert for using [https://web.esphome.io/](https://web.esphome.io/)

## Flashing Instructions - Command line

### Install esptool
```
pip install esptool
```

### Find your serial port
- macOS / Linux: `ls /dev/tty.usb*`
- Windows: Device Manager → Ports (COM & LPT)

### Flash Command
Replace `PORT` with your actual serial port (e.g. `/dev/tty.usbserial-0001` on macOS, `COM3` on Windows):

```bash
esptool --chip esp32 --port PORT --baud 921600 write-flash 0x1000 v1.4.0.bin
```

### Notes
- Hold the BOOT button on the device before flashing, or ensure the device is in download mode <-- THIS IS NOT NECESSARY ON THE FEATHER ESP32 PLATFORM WE'RE USING
- If flashing fails, try a lower baud rate (e.g. `--baud 115200`)


# ESP8266 Firmware - legacy

*add guidelines* for non expert for using [https://web.esphome.io/](https://web.esphome.io/)


