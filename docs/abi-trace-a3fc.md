# ABI trace — A3FC / index255

- Status: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` on contractual return only.
- AS3000 2005: non-pointer/non-callable table value.
- NEO 2005: non-pointer/non-callable table value.
- NEO 2013: callable handler at runtime `0x0043409C`.
- Handler boundary: `0x162` bytes; internal 8-word dispatch table; one terminal RTS for the handler.
- Handler SHA-256: `b039c5eb7824a83caa675ee779cf4ec5a02f85439509b883b81b85f20ad29bb8`.
- Physical ABI: four 32-bit slots, consumed as pointer / low8 / low8 selector / low8 bit source.
- Mechanical selector domain: `0..7`; out-of-range path performs no indexed bit update.
- Selector-to-bit map: `0→7, 1→6, 2→5, 3→0, 4→4, 5→3, 6→2, 7→1`.
- Direct NEO 2013 firmware callers: 2; no positive contractual D0 consumption demonstrated.
- Official SmartApplet callers: 0/41 by structural absence of the late slot.
- Historical/public symbol or prototype: none recovered.
- Static regression: `27/27 PASS / OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.
- Publication rule: no callable header until return intent/type is independently demonstrated.
