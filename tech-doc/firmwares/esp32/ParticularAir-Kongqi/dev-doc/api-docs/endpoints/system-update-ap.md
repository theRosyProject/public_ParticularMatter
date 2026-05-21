# POST /api/system/update-ap

Updates the AP hotspot SSID and password. A reboot is required for the changes to take effect.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/system/update-ap` |
| Method | POST |
| Content-Type | application/json |
| Auth | None |

### Request Body

| Field | Type | Required | Description |
| --- | --- | --- | --- |
| `ssid` | String | Yes | New AP SSID |
| `password` | String | Yes | New AP password |

### Request Example

```json
{
  "ssid": "MyNode-AP",
  "password": "newpassword123"
}
```

## Response (`data`)

```json
{}
```
