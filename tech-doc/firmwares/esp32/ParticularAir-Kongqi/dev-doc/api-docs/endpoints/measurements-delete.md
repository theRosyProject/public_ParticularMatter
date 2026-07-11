# POST /api/measurement/delete

Deletes a single daily measurement CSV file from the SD card by filename.

Use [`GET /api/measurements`](./measurements-list.md) to obtain the list of available filenames before calling this endpoint.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/measurement/delete` |
| Method | POST |
| Content-Type | `application/json` |
| Auth | None |

### Request Body

| Field | Type | Required | Description |
| --- | --- | --- | --- |
| `name` | String | Yes | Filename of the measurement file to delete (e.g. `2026-05-11_log.csv`) |

### Request Example

```json
{
  "name": "2026-05-11_log.csv"
}
```

## Response (`data`)

```json
{}
```

## Errors

| HTTP | `message` | Cause |
| --- | --- | --- |
| 400 | `Missing field: name` | The `name` field was not provided in the request body |
| 404 | `File not found` | No file with the given name exists on the SD card |
| 503 | `SD card not ready` | SD card unavailable or not mounted |