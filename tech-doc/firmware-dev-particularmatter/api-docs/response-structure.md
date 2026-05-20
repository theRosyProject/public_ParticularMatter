# API Response Structure

All API responses are wrapped in the following envelope.

```json
{
  "success": true,
  "data": {} | [],
  "message": "",
  "timestamp": "yyyy/mm/dd HH:MM:SS"
}
```

| Field | Type | Description |
| --- | --- | --- |
| `success` | Boolean | `true` on success, `false` on failure |
| `data` | Object \| Array | Endpoint-specific payload (see individual endpoint docs) |
| `message` | String | Error or informational message; empty string on success |
| `timestamp` | String | Device local time, format `yyyy/mm/dd HH:MM:SS` |
