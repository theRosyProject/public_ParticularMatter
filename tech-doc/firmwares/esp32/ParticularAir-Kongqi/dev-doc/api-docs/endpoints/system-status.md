# GET /api/system/status

Returns the current system, WiFi, and node pairing status of the ESP32.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/system/status` |
| Method | GET |
| Content-Type | N/A |
| Auth | None |

## Response (`data`)

| Field | Type | Description |
| --- | --- | --- |
| `systemStatus` | [systemType](../../types/systemType.md) | Firmware and AP information |
| `wifiStatus` | [wifiType](../../types/wifiType.md) | WiFi connection information |
| `nodeStatus` | [nodeType](../../types/nodeType.md) | Node pairing information |

### Response Example

```json
{
  "systemStatus": {
    "firmwareName": "ParticularMatter-Node",
    "firmwareVersion": "v2.0.0",
    "macAddress": "24:6F:28:AE:B1:0C",
    "isPasswordChanged": true,
    "firmwareSsid": "ESP-32 Setup",
    "allowedSkipInit": false,
  },
  "wifiStatus": {
    "ssid": "MyHomeNetwork_5G",
    "ipAddress": "192.168.1.150",
    "macAddress": "24:6F:28:AE:B1:0C",
    "rssi": -55,
    "connectionState": "CONNECTED"
  },
  "nodeStatus": {
    "nodeId": "a7f34d1b-25b2-4e9c-93b3-2ac9d8c7e218",
    "pairingState": "REGISTERED"
  }
}
```
