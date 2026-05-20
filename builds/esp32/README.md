# v1.4.0

## Flashing Instructions

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
- Hold the BOOT button on the device before flashing, or ensure the device is in download mode
- If flashing fails, try a lower baud rate (e.g. `--baud 115200`)
