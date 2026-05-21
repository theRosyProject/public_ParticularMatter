# GET /api/measurements

Returns the list of daily measurement CSV files currently stored on the SD card under `/measurements/`.

Use this endpoint to populate a download UI (e.g. a list of dates with file sizes). To actually fetch a file, call [`GET /api/measurements/file`](./measurements-file.md). To download every file as a single concatenated CSV, call [`GET /api/measurements/all`](./measurements-all.md).

## Request

| Field | Value |
| --- | --- |
| URL | `/api/measurements` |
| Method | GET |
| Content-Type | N/A |
| Auth | None |

### Request Body

None.

## Response (`data`)

| Field | Type | Description |
| --- | --- | --- |
| `count` | Number | Number of measurement files available |
| `totalBytes` | Number | Sum of all file sizes in bytes |
| `files` | Array(Object) | Per-file metadata (see below) |

### `files[]` item

| Field | Type | Description |
| --- | --- | --- |
| `name` | String | Filename, format `YYYY-MM-DD_log.csv` |
| `size` | Number | File size in bytes |
| `date` | String | ISO date extracted from the filename (`YYYY-MM-DD`) |

### Response Example

```json
{
  "count": 3,
  "totalBytes": 912345,
  "files": [
    { "name": "2026-05-11_log.csv", "size": 287010, "date": "2026-05-11" },
    { "name": "2026-05-12_log.csv", "size": 312456, "date": "2026-05-12" },
    { "name": "2026-05-13_log.csv", "size": 312879, "date": "2026-05-13" }
  ]
}
```

> **Note:** Files are sorted by filename, which — given the `YYYY-MM-DD` prefix — is equivalent to chronological order ascending.

## Errors

| HTTP | `message` | Cause |
| --- | --- | --- |
| 503 | `SD card not ready` | SD card unavailable or not mounted |

When the `/measurements/` directory does not exist yet (no measurement has ever been written), the endpoint still returns `200` with `count: 0` and an empty `files` array.
