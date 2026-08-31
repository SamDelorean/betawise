# ABI trace — A3FC / index255

- Status: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` on contractual return only.
- AS3000 2005: `0x5A06004E`, non-pointer/non-callable table value.
- NEO 2005: `0x7F08005E`, non-pointer/non-callable table value.
- NEO 2013: callable handler at runtime `0x0043409C`, file offset `0x02409C`.
- Handler boundary: `0x162` bytes; 107 executable instructions plus an internal 8-word dispatch table; one terminal RTS.
- Handler SHA-256: `b039c5eb7824a83caa675ee779cf4ec5a02f85439509b883b81b85f20ad29bb8`.
- Physical ABI: four 32-bit slots, consumed as pointer / low8 index / low8 selector / low8 bit source.
- Index mechanics: `record = arg1 + 4 + signext8(arg2) * 0x12C`.
- Mechanical selector domain: `0..7`; out-of-range performs no indexed bit update.
- Correct selector-to-bit map at record byte `+0x10D`: `0→7, 1→6, 2→5, 3→4, 4→3, 5→2, 6→1, 7→0`.
- Correction note: the first concurrent trace mistakenly read the physical case order instead of resolving the jump-table targets for selectors 3–7; this entry supersedes that transcription.
- Direct NEO 2013 firmware callers: exactly 2, at file offsets `0x02E2DC` and `0x02E33C`; shapes `[0x12CA0,0,5,1]` and `[0x12CA0,0,5,0]`; no contractual D0 consumption demonstrated.
- Official SmartApplet callers: `0/41` by structural absence of the late slot.
- Historical/public symbol or prototype: none recovered.
- Extended static regression: `36/36 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.
- Publication rule: no callable header until return intent/type is independently demonstrated.
