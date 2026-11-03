# systemType

Type definition for the `systemStatus` field in `GET /api/system/status`.

| Field | Type | Description |
| --- | --- | --- |
| `firmwareName` | String | Always `"ParticularMatter-Node"` |
| `firmwareVersion` | String | e.g. `"v2.0.0"` |
| `macAddress` | String | ESP32 MAC address |
| `isPasswordChanged` | Boolean | `false` means the AP password is still at factory default |
| `firmwareSsid` | String | Current AP hotspot SSID |
| `allowedSkipInit` | Boolean | Status of allowing skip initialize the configuration |
