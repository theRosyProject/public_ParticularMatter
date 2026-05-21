# MQTT Format

## Topic

```
measurements/{node_id}/{sensor_id}
```

`node_id` and `sensor_id` come from pairing ([`POST /api/node/save`](../api-docs/endpoints/node-save.md)).

## Publish

- Interval: 5 seconds
- QoS: 0
- Retain: true
- Sent only after pairing is complete and at least one sensor has a valid reading.

## Payload

```json
{
  "measurement": {
    "pm1": 12,
    "pm25": 18,
    "pm10": 25,
    "co2": 493,
    "temperature": 24.50,
    "humidity": 55.20,
    "pressure": 1013.10,
    "gps": {
      "lat": 44.523474,
      "lng": 11.337641,
      "alt": 35.2,
      "hdop": 1.20,
      "sats": 7,
      "fix": 1
    }
  },
  "validity": { "pm": true, "co2": true, "env": true, "gps": true },
  "datetime": "2026-05-12T14:02:42Z",
  "ts_ms": 87234
}
```

## Fields

| Field                     | Type             | Unit / Meaning                          |
| ------------------------- | ---------------- | --------------------------------------- |
| `measurement.pm1`         | `number \| null` | PM1.0, μg/m³                            |
| `measurement.pm25`        | `number \| null` | PM2.5, μg/m³                            |
| `measurement.pm10`        | `number \| null` | PM10, μg/m³                             |
| `measurement.co2`         | `number \| null` | CO₂, ppm                                |
| `measurement.temperature` | `number \| null` | °C                                      |
| `measurement.humidity`    | `number \| null` | %RH                                     |
| `measurement.pressure`    | `number \| null` | hPa                                     |
| `measurement.gps.lat`     | `number \| null` | latitude, degrees                       |
| `measurement.gps.lng`     | `number \| null` | longitude, degrees                      |
| `measurement.gps.alt`     | `number \| null` | altitude, m                             |
| `measurement.gps.hdop`    | `number \| null` | horizontal dilution of precision        |
| `measurement.gps.sats`    | `number \| null` | satellites in fix                       |
| `measurement.gps.fix`     | `0 \| 1`         | 1 if GPS has a fix                      |
| `validity.pm`             | `boolean`        | PMS5003 reading valid                   |
| `validity.co2`            | `boolean`        | S8 reading valid                        |
| `validity.env`            | `boolean`        | BME280 reading valid                    |
| `validity.gps`            | `boolean`        | GPS has a fix                           |
| `datetime`                | `string`         | GPS UTC, `YYYY-MM-DDTHH:MM:SSZ` (empty if no time) |
| `ts_ms`                   | `number`         | `millis()` since boot                   |

Invalid sensor fields are `null`. `ts_ms` is uptime in milliseconds, not epoch time.
