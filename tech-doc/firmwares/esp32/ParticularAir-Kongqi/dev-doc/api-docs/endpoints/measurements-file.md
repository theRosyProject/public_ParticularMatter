# GET /api/measurements/file

Downloads a single daily measurement CSV file from the SD card.

Unlike most endpoints, this one does **not** return a JSON envelope: the body is the raw CSV file streamed as `text/csv` with `Transfer-Encoding: chunked` so the firmware never loads the whole file into RAM.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/measurements/file` |
| Method | GET |
| Content-Type | N/A |
| Auth | None |

### Query parameters

| Field | Type | Required | Description |
| --- | --- | --- | --- |
| `name` | String | Yes | Filename to download. Must match the pattern `YYYY-MM-DD_log.csv` (the names returned by [`GET /api/measurements`](./measurements-list.md)). |

### Example request

```
GET /api/measurements/file?name=2026-05-13_log.csv
```

## Response

### Success — `200 OK`

| Header | Value |
| --- | --- |
| `Content-Type` | `text/csv; charset=utf-8` |
| `Content-Disposition` | `attachment; filename="<name>"` |
| `Cache-Control` | `no-cache` |
| `Transfer-Encoding` | `chunked` |

The body is the verbatim CSV file. The first line is the canonical header; every subsequent line is one measurement row.

#### Example body

```csv
gps_datetime,wifi_datetime,ts_ms,pm1,pm25,pm10,co2,temperature,humidity,pressure,lat,lng,altitude,hdop,satellites,gps_valid
2026-05-13T04:00:01Z,2026-05-13T04:00:01Z,12345,5,8,12,420,26.31,58.12,1012.45,25.034000,121.564000,12.3,0.90,9,1
2026-05-13T04:00:11Z,2026-05-13T04:00:11Z,22468,6,9,13,418,26.34,58.05,1012.43,25.034012,121.564021,12.4,0.90,9,1
```

## Errors

Errors use the standard JSON envelope.

| HTTP | `message` | Cause |
| --- | --- | --- |
| 400 | `Missing 'name' query parameter` | `name` was not supplied |
| 400 | `Invalid filename` | `name` does not match `YYYY-MM-DD_log.csv` |
| 404 | `File not found` | File does not exist on the SD card |
| 500 | `Failed to open file` | SD I/O error while opening the file |
| 503 | `SD card not ready` | SD card unavailable or not mounted |

> **Security note:** Only filenames that match the strict pattern `YYYY-MM-DD_log.csv` are accepted; any other input (including paths containing `/` or `..`) returns `400` without touching the filesystem.

> **Note:** The device pauses sensor sampling and MQTT publishing for the duration of the transfer (the HTTP server runs on the main loop). Large downloads complete in seconds over the AP — keep this in mind for the UI.
