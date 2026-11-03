# POST /api/wifi/scan

Triggers a background WiFi scan on the device.

After calling this endpoint, the client should start polling [`GET /api/wifi/ssid`](./wifi-ssid.md) every 2 seconds until `isFinished` is `true`.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/wifi/scan` |
| Method | POST |
| Content-Type | application/json |
| Auth | None |

### Request Body

None.

## Response (`data`)

```json
{}
```
