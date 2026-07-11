# Sensors and Wiring

Board: Adafruit ESP32 Feather (HUZZAH32).

| Sensor      | Interface | Pins                                    |
| ----------- | --------- | --------------------------------------- |
| PMS5003     | UART      | RX = 13                                 |
| SenseAir S8 | I²C       | SDA = 23, SCL = 22, address `0x68`      |
| BME280      | I²C       | SDA = 23, SCL = 22, address `0x76`/`0x77` |
| NEO-6M GPS  | UART      | RX = 14, TX = 27                        |
| MicroSD     | SPI       | CS = 33, SCK = 18, MOSI = 23, MISO = 19 |

## Power

- PMS5003: 5 V
- SenseAir S8: 5 V
- BME280: 3.3 V (or 5 V on Adafruit breakout)
- NEO-6M GPS: 3.3 V or 5 V
- MicroSD: 3.3 V

## Notes

- PMS5003 baud rate: 9600.
- NEO-6M baud rate: 9600.
- BME280 driver auto-detects between `0x76` and `0x77`.
- GPIO 12 is reserved (boot strapping pin) — don't use it.
