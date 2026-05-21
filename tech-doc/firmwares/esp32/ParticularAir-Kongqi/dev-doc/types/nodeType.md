# nodeType

Type definition for the `nodeStatus` field in `GET /api/system/status`.

| Field | Type | Description |
| --- | --- | --- |
| `nodeId` | String | Node ID on the ParticularMatter platform; empty string before pairing |
| `pairingState` | [PAIRING_STATE](../enums/PAIRING_STATE.md) | Node pairing state |
