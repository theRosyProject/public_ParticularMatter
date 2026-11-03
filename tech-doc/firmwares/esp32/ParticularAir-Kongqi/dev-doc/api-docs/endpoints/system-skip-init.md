# POST /api/system/skip-init

Sets `allowedSkipInit` to `true` on the ESP32, allowing the device to skip the initialization step on next boot.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/system/skip-init` |
| Method | POST |
| Content-Type | application/json |
| Auth | None |

### Request Body

None.

## Response (`data`)

```json
{}
```