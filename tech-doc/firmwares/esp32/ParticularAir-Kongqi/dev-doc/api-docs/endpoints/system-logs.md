# GET /api/system/logs

Stream the in-memory system log ring buffer.

Unlike the rest of the API, this endpoint does **not** return a JSON envelope.
The body is the raw log lines as `text/plain`, transferred with
`Transfer-Encoding: chunked`. The firmware keeps the most recent **200**
lines in a fixed-size RAM ring buffer; this endpoint reads directly from
that buffer and does **not** touch the SD card, so it works even when no SD
card is inserted and never blocks on SD I/O.

The device also writes a longer history (up to ~2000 lines) to
`/log/system.log` on the SD card when one is present, but that file is not
served by this endpoint.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/system/logs` |
| Method | GET |
| Content-Type | N/A |
| Auth | None |

No request body.

## Response

### Success — `200 OK`

| Header | Value |
| --- | --- |
| `Content-Type` | `text/plain; charset=utf-8` |
| `Content-Disposition` | `inline; filename="esp32_system.log"` |
| `Cache-Control` | `no-cache` |
| `Transfer-Encoding` | `chunked` |

Each line uses the format written by the firmware logger:

```
[+<ms> ms] [<LEVEL>] <message>
```

`<ms>` is `millis()` since boot (the device has no wall-clock source).
`<LEVEL>` is one of `I`, `W`, `E`, `D`.

#### Example body

```text
[+         0 ms] [I] Logger: file=/log/system.log lines=0 max=1000 buf=1024B interval=2000ms ring=200
[+       142 ms] [I] Web portal started at 192.168.4.1
[+      3050 ms] [W] LittleFS mount failed, attempting format...
[+      5210 ms] [I] API /system/status: received request
[+   1234567 ms] [E] SD write failed at /data/2026-05-13.csv
```

### Error — `503 Service Unavailable`

Returned only if the logger has not been initialized yet (should not happen
in normal operation). The body is plain text (no JSON envelope):

```text
Log unavailable: logger not started
```

## Notes

- Lines are served oldest → newest, in the order they were emitted.
- The ring buffer holds the most recent **200** lines; older history is
  discarded from RAM (but may still be on the SD card under
  `/log/system.log` / `/log/system.log.1`).
- Each `<message>` is truncated to **96 characters** when stored in the
  ring; the SD copy is not truncated.
- The ring buffer is **cleared on reboot** — only the SD copy survives a
  restart.
- Front-end clients should consume the body via `Response.text()` or stream
  it directly to a file download; do **not** attempt to parse it as JSON.