# POST /api/wifi/save

Saves WiFi credentials and triggers a connection attempt. The device connects to the specified network in STA mode. Use [`GET /api/system/status`](./system-status.md) to monitor `connectionState` for the result.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/wifi/save` |
| Method | POST |
| Content-Type | application/json |
| Auth | None |

### Request Body

| Field | Type | Required | Description |
| --- | --- | --- | --- |
| `ssid` | String | Yes | Target WiFi SSID |
| `password` | String | Yes | WiFi password; pass an empty string `""` for open networks |

### Request Example

```json
{
  "ssid": "MyHomeNetwork_5G",
  "password": "mysecretpassword"
}
```

## Response (`data`)

```json
{}
```
