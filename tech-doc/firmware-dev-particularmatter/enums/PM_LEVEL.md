# PM_LEVEL

Quality level enum for particulate matter fields (`pm1`, `pm25`, `pm10`) in the [`/api/measurements/latest`](../api-docs/endpoints/measurements-latest.md) response.

Thresholds follow the **Taiwan EPA AQI** breakpoints for PM2.5 and PM10. PM1 has no official standard and uses the PM2.5 breakpoints as a proxy.

| Value | Color | PM2.5 (µg/m³) | PM10 (µg/m³) |
| --- | --- | --- | --- |
| `GOOD` | 🟢 Green | 0 – 15.4 | 0 – 54 |
| `MODERATE` | 🟡 Yellow | 15.5 – 35.4 | 55 – 124 |
| `UNHEALTHY_SENSITIVE` | 🟠 Orange | 35.5 – 54.4 | 125 – 254 |
| `UNHEALTHY` | 🔴 Red | 54.5 – 150.4 | 255 – 354 |
| `VERY_UNHEALTHY` | 🟣 Purple | 150.5 – 250.4 | 355 – 424 |
| `HAZARDOUS` | 🟤 Brown | ≥ 250.5 | ≥ 425 |
