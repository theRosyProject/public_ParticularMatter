# API Documentation

All Web Portal APIs are served over HTTP/1.1. Once the device boots, they are accessible via the AP hotspot (default IP: `192.168.4.1`).

## Common Response Envelope

All responses are wrapped in the same outer structure. See [response-structure.md](./response-structure.md) for details.

```json
{
  "success": true,
  "data": {},
  "message": "",
  "timestamp": "yyyy/mm/dd HH:MM:SS"
}
```

> **Note:** The response examples in each endpoint document show only the contents of the `data` field.

---

## Endpoints

### System

| Method | Endpoint | Description |
| --- | --- | --- |
| GET | [/api/system/status](./endpoints/system-status.md) | Get device system status |
| POST | [/api/system/reboot](./endpoints/system-reboot.md) | Reboot the device |
| POST | [/api/system/reset](./endpoints/system-reset.md) | Factory reset (clears all saved settings) |
| POST | [/api/system/update-ap](./endpoints/system-update-ap.md) | Update AP hotspot SSID and password |
| GET | [/api/system/logs](./endpoints/system-logs.md) | Stream the on-device system log (plain text) |

### WiFi

| Method | Endpoint | Description |
| --- | --- | --- |
| POST | [/api/wifi/scan](./endpoints/wifi-scan.md) | Trigger a background WiFi scan |
| GET | [/api/wifi/ssid](./endpoints/wifi-ssid.md) | Poll for WiFi scan results |
| POST | [/api/wifi/save](./endpoints/wifi-save.md) | Save WiFi credentials and connect |

### Node

| Method | Endpoint | Description |
| --- | --- | --- |
| POST | [/api/node/save](./endpoints/node-save.md) | Pair the device using a one-time key |

---

## Measurements

| Method | Endpoint | Description |
| --- | --- | --- |
| GET | [/api/measurements](./endpoints/measurements-list.md) | List daily measurement CSV files on the SD card |
| GET | [/api/measurements/latest](./endpoints/measurements-latest.md) | Get the latest sensor snapshot with quality levels |
| GET | [/api/measurements/file](./endpoints/measurements-file.md) | Download a single daily measurement CSV file |
| GET | [/api/measurements/all](./endpoints/measurements-all.md) | Download all measurement files concatenated into one CSV |
| POST | [/api/measurements/delete](./endpoints/measurements-delete.md) | Delete a single daily measurement CSV file |

---

## Types & Enums

### Types

| Type | Description |
| --- | --- |
| [systemType](../types/systemType.md) | System status object |
| [wifiType](../types/wifiType.md) | WiFi status object |
| [nodeType](../types/nodeType.md) | Node pairing status object |
| [networkType](../types/networkType.md) | Single network entry from a WiFi scan |

### Enums

| Enum | Description |
| --- | --- |
| [CONNECTION_STATE](../enums/CONNECTION_STATE.md) | WiFi connection state values |
| [PAIRING_STATE](../enums/PAIRING_STATE.md) | Node pairing state values |
