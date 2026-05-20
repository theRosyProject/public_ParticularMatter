# PAIRING_STATE

Enum values for the `nodeType.pairingState` field.

| Value | Description |
| --- | --- |
| `UNREGISTERED` | Not yet paired (fresh boot or after a factory reset) |
| `REGISTERING` | Device is executing the HTTPS pairing request in the background |
| `REGISTERED` | Pairing successful; MQTT credentials have been obtained |
| `INVALID_KEY` | Server returned 400 / 401 / 403 (key invalid or expired) |
| `NETWORK_ERROR` | Server unreachable, timeout, or TLS failure |
