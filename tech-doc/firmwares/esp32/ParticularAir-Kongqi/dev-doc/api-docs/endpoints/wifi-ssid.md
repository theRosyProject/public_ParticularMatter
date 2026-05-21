# GET /api/wifi/ssid

Returns the latest WiFi scan results.

Call [`POST /api/wifi/scan`](./wifi-scan.md) first to trigger a scan, then poll this endpoint every 2 seconds until `isFinished` is `true`.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/wifi/ssid` |
| Method | GET |
| Content-Type | N/A |
| Auth | None |

## Response (`data`)

| Field | Type | Description |
| --- | --- | --- |
| `isFinished` | Boolean | `true` when the scan is complete; client may stop polling |
| `networks` | Array([networkType](../../types/networkType.md)) | List of discovered networks |

### Response Example

```json
{
  "isFinished": true,
  "networks": [
    {
      "ssid": "Home_5G",
      "rssi": -45,
      "isSecure": true,
      "bssid": "24:4B:FE:BC:DE:F0"
    },
    {
      "ssid": "Free_WiFi",
      "rssi": -82,
      "isSecure": false,
      "bssid": "AA:BB:CC:DD:EE:FF"
    }
  ]
}
```

> **Note:** `rssi` is in dBm. Values closer to 0 indicate stronger signal (e.g. -45 is better than -82).
