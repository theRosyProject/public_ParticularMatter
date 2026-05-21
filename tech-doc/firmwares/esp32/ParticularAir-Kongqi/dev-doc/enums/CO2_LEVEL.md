# CO2_LEVEL

Quality level enum for the `co2` field in the [`/api/measurements/latest`](../api-docs/endpoints/measurements-latest.md) response.

Thresholds follow the **RESET Air** standard.

| Value | Color | ppm |
| --- | --- | --- |
| `GOOD` | 🟢 Green | < 600 |
| `MODERATE` | 🟡 Yellow | 600 – 999 |
| `POOR` | 🟠 Orange | 1000 – 1499 |
| `VERY_POOR` | 🔴 Red | ≥ 1500 |
