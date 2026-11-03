# GET /api/measurements/all

Downloads every daily measurement CSV file on the SD card, concatenated into a single CSV. The canonical header is emitted once at the top and the per-file header lines are stripped from each subsequent file, so the output is a valid single CSV that contains every row across all days.

Like [`GET /api/measurements/file`](./measurements-file.md), this endpoint does **not** return a JSON envelope: the body is raw `text/csv` streamed with `Transfer-Encoding: chunked`.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/measurements/all` |
| Method | GET |
| Content-Type | N/A |
| Auth | None |

### Request Body

None.

## Response

### Success — `200 OK`

| Header | Value |
| --- | --- |
| `Content-Type` | `text/csv; charset=utf-8` |
| `Content-Disposition` | `attachment; filename="measurements_all.csv"` |
| `Cache-Control` | `no-cache` |
| `Transfer-Encoding` | `chunked` |

Files are emitted in filename order, which — given the `YYYY-MM-DD` prefix — is chronological ascending. Every row already carries its own `gps_datetime` / `wifi_datetime`, so no per-file separator is needed.

#### Example body

```csv
gps_datetime,wifi_datetime,ts_ms,pm1,pm25,pm10,co2,temperature,humidity,pressure,lat,lng,altitude,hdop,satellites,gps_valid
2026-05-11T03:59:50Z,2026-05-11T03:59:50Z,1000,4,7,11,415,26.10,57.80,1012.50,25.034000,121.564000,12.1,0.90,9,1
2026-05-11T04:00:00Z,2026-05-11T04:00:00Z,11200,5,7,12,416,26.12,57.82,1012.49,25.034000,121.564000,12.1,0.90,9,1
2026-05-12T04:00:01Z,2026-05-12T04:00:01Z,12345,5,8,12,420,26.31,58.12,1012.45,25.034012,121.564021,12.3,0.90,9,1
2026-05-13T04:00:01Z,2026-05-13T04:00:01Z,12345,6,9,13,418,26.34,58.05,1012.43,25.034000,121.564000,12.4,0.90,9,1
```

If no measurement files exist, the response is still `200 OK` and contains only the header line.

## Errors

Errors use the standard JSON envelope.

| HTTP | `message` | Cause |
| --- | --- | --- |
| 503 | `SD card not ready` | SD card unavailable or not mounted |

> **Note:** The device pauses sensor sampling and MQTT publishing for the duration of the transfer (the HTTP server runs on the main loop). For deployments with many days of accumulated data, prefer per-file downloads via [`/api/measurements/file`](./measurements-file.md) so each transfer completes quickly and is independently retryable.
