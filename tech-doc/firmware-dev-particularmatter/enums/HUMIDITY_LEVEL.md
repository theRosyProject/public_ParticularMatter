# HUMIDITY_LEVEL

Quality level enum for the `humidity` field in the [`/api/measurements/latest`](../api-docs/endpoints/measurements-latest.md) response.

Thresholds are based on **ASHRAE 55 / EN 15251**.

| Value | Color | %RH |
| --- | --- | --- |
| `DRY` | 🟠 Orange | < 30 |
| `COMFORTABLE` | 🟢 Green | 30 – 60 |
| `HUMID` | 🟡 Yellow | 61 – 80 |
| `VERY_HUMID` | 🔴 Red | > 80 |
