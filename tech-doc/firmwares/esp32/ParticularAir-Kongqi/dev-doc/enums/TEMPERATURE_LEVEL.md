# TEMPERATURE_LEVEL

Quality level enum for the `temperature` field in the [`/api/measurements/latest`](../api-docs/endpoints/measurements-latest.md) response.

Thresholds are based on the **ASHRAE 55** thermal comfort zone.

| Value | Color | °C |
| --- | --- | --- |
| `COLD` | 🔵 Blue | < 17 |
| `COOL` | 🟡 Yellow | 17 – 19 |
| `COMFORTABLE` | 🟢 Green | 20 – 26 |
| `WARM` | 🟡 Yellow | 27 – 30 |
| `HOT` | 🔴 Red | > 30 |
