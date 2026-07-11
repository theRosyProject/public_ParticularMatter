# GET /api/measurements/latest

Returns the most recent sensor snapshot collected by the device — one entry per sensor field, including its current value, unit, the number of samples aggregated in the last MQTT interval for that sensor group, and a quality level string.

Because each sensor driver polls hardware at its own rate (e.g. PMS5003 every 3 s, BME280 every 2 s), the `samplesInInterval` value can differ between sensor groups within the same MQTT interval.

## Request

| Field | Value |
| --- | --- |
| URL | `/api/measurements/latest` |
| Method | GET |
| Content-Type | N/A |
| Auth | None |

### Request Body

None.

---

## Response (`data`)

### Top-level fields

| Field | Type | Description |
| --- | --- | --- |
| `mqttIntervalMs` | Number | MQTT publish interval in milliseconds (e.g. `1000`) |
| `pm1` | [SensorField](#sensorfield-object) | PM1.0 particulate matter |
| `pm25` | [SensorField](#sensorfield-object) | PM2.5 particulate matter |
| `pm10` | [SensorField](#sensorfield-object) | PM10 particulate matter |
| `co2` | [SensorField](#sensorfield-object) | CO₂ concentration |
| `temperature` | [SensorField](#sensorfield-object) | Ambient temperature |
| `humidity` | [SensorField](#sensorfield-object) | Relative humidity |
| `pressure` | [SensorField](#sensorfield-object) | Atmospheric pressure (no `level`, always `null`) |

### `SensorField` object

| Field | Type | Description |
| --- | --- | --- |
| `value` | Number \| `null` | Sensor reading. `null` when `valid` is `false` |
| `unit` | String | Physical unit of the value (see table below) |
| `valid` | Boolean | `true` if the sensor produced a valid reading in the last interval |
| `samplesInInterval` | Number | Number of hardware polls that were successfully aggregated by this sensor's driver in the last MQTT interval. Shared across fields that belong to the same physical sensor (pm1/pm25/pm10 share one count; co2 has its own; temperature/humidity/pressure share one) |
| `level` | [PM_LEVEL](../../enums/PM_LEVEL.md) \| [CO2_LEVEL](../../enums/CO2_LEVEL.md) \| [TEMPERATURE_LEVEL](../../enums/TEMPERATURE_LEVEL.md) \| [HUMIDITY_LEVEL](../../enums/HUMIDITY_LEVEL.md) \| `null` | Quality level enum value derived from the reading. `null` when `valid` is `false` or the field has no defined threshold (`pressure`). See the enum reference table below for which enum applies to each field |

#### Units per field

| Field | Unit |
| --- | --- |
| `pm1`, `pm25`, `pm10` | `µg/m³` |
| `co2` | `ppm` |
| `temperature` | `°C` |
| `humidity` | `%RH` |
| `pressure` | `hPa` |

---

## Level enums

| Field | Enum | Reference |
| --- | --- | --- |
| `pm1`, `pm25`, `pm10` | [PM_LEVEL](../../enums/PM_LEVEL.md) | Taiwan EPA AQI; PM1 uses PM2.5 breakpoints as proxy |
| `co2` | [CO2_LEVEL](../../enums/CO2_LEVEL.md) | RESET Air standard |
| `temperature` | [TEMPERATURE_LEVEL](../../enums/TEMPERATURE_LEVEL.md) | ASHRAE 55 comfort zone |
| `humidity` | [HUMIDITY_LEVEL](../../enums/HUMIDITY_LEVEL.md) | ASHRAE 55 / EN 15251 |
| `pressure` | — | No threshold defined; `level` is always `null` |

---

## Response Example

```json
{
  "mqttIntervalMs": 10000,
  "pm1":         { "value": 4,       "unit": "µg/m³", "valid": true,  "samplesInInterval": 3, "level": "GOOD" },
  "pm25":        { "value": 7,       "unit": "µg/m³", "valid": true,  "samplesInInterval": 3, "level": "GOOD" },
  "pm10":        { "value": 11,      "unit": "µg/m³", "valid": true,  "samplesInInterval": 3, "level": "GOOD" },
  "co2":         { "value": 415,     "unit": "ppm",   "valid": true,  "samplesInInterval": 3, "level": "GOOD" },
  "temperature": { "value": 26.10,   "unit": "°C",    "valid": true,  "samplesInInterval": 5, "level": "COMFORTABLE" },
  "humidity":    { "value": 57.80,   "unit": "%RH",   "valid": true,  "samplesInInterval": 5, "level": "COMFORTABLE" },
  "pressure":    { "value": 1012.50, "unit": "hPa",   "valid": true,  "samplesInInterval": 5, "level": null }
}
```

### When sensors are not yet ready

If the device has just booted and no valid reading has been collected yet, all `SensorField` entries will have `valid: false`, `value: null`, `samplesInInterval: 0`, and `level: null`.

```json
{
  "mqttIntervalMs": 10000,
  "pm1":         { "value": null, "unit": "µg/m³", "valid": false, "samplesInInterval": 0, "level": null },
  "pm25":        { "value": null, "unit": "µg/m³", "valid": false, "samplesInInterval": 0, "level": null },
  "pm10":        { "value": null, "unit": "µg/m³", "valid": false, "samplesInInterval": 0, "level": null },
  "co2":         { "value": null, "unit": "ppm",   "valid": false, "samplesInInterval": 0, "level": null },
  "temperature": { "value": null, "unit": "°C",    "valid": false, "samplesInInterval": 0, "level": null },
  "humidity":    { "value": null, "unit": "%RH",   "valid": false, "samplesInInterval": 0, "level": null },
  "pressure":    { "value": null, "unit": "hPa",   "valid": false, "samplesInInterval": 0, "level": null }
}
```

---

## Errors

| HTTP | `message` | Cause |
| --- | --- | --- |
| 503 | `Sensor system not initialised` | Called before the sensor loop has run at least once |
