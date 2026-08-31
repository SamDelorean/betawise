# Forward-sweep trace — A3D4 / index 245

- Status: mechanically reconstructed A; blocked exclusively on contractual return intent.
- AS3000 2005 raw slot: `0x5B5E004E`, non-pointer/unresolved.
- NEO 2005 raw slot: `0x8068005E`, non-pointer/unresolved.
- NEO 2013 handler: runtime `0x0043F508`, file `0x02F508`, length `0x3A`, 19 decoded instructions, SHA-256 `c7ff7ac9d4fc4f34d2d1881238a0c3b88c8c6275813200050ffc9e67f0bc3aec`.
- Physical input ABI: one 32-bit source-pointer slot.
- Internal count: big-endian 16-bit value from two state bytes; destination is an internal buffer.
- Direct dependency: A360/`memcpy`.
- Mechanical terminal D0.L: inherited upper word from `memcpy` plus count in low word; no contractual width inferred.
- NEO 2013 direct JSR/JMP/BSR.W callers: 0/0/0.
- Official SmartApplet corpus: 41/41 structurally checked; demonstrated extended tables stop at A3BC, so A3D4 has no A-line slot and callers are 0/41.
- No independent BetaWise/neo-re symbol or prototype recovered.
- Static regression: OVERALL PASS. Dynamic regression: specified, not executed.

Public disposition: [`sys-a3d4-blocked.md`](sys-a3d4-blocked.md).

Detailed firmware/disassembly evidence remains private in Drive.
