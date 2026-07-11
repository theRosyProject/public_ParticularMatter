# POST /api/system/reset

Factory resets the ESP32 by clearing all saved settings. The device reboots automatically after the reset completes.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/system/reset` |
| Method | POST |
| Content-Type | application/json |
| Auth | None |

### Request Body

None.

## Response (`data`)

```json
{}
```
