# wifiType

Type definition for the `wifiStatus` field in `GET /api/system/status`.

| Field | Type | Description |
| --- | --- | --- |
| `ssid` | String | Currently connected SSID; empty string when not connected |
| `ipAddress` | String | Device IP address in STA mode; empty string when not connected |
| `macAddress` | String | ESP32 MAC address |
| `rssi` | Number | Signal strength in dBm; `0` when not connected |
| `connectionState` | [CONNECTION_STATE](../enums/CONNECTION_STATE.md) | WiFi connection state |
