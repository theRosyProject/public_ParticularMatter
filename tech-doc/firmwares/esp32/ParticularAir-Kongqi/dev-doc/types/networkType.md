# networkType

Type definition for each element in the `networks` array returned by [`GET /api/wifi/ssid`](../api-docs/endpoints/wifi-ssid.md).

| Field | Type | Description |
| --- | --- | --- |
| `ssid` | String | Network SSID |
| `rssi` | Number | Signal strength in dBm; closer to 0 means stronger signal |
| `isSecure` | Boolean | `true` if the network requires a password; `false` for open networks |
| `bssid` | String | Access point MAC address |
