# NEO Manager selector 0x09 — baud-rate change across principal OS and Small ROM

This note records a source-first and firmware-correlated reconstruction of Manager selector `0x09` for NEO System 3. It intentionally distinguishes the principal OS implementation from the NEO Small ROM implementation instead of forcing their error policies to be identical.

## Source-first identification

AlphaSync names request `0x09` as `ASMESSAGE_REQUEST_SET_BAUDRATE` and describes its command field as `(baud32, z16)`. It names response `0x4A` as the baud-rate response. AlphaSync also records `0x86` as the invalid-baudrate error used by the normal Manager protocol implementation.

The canonical firmware independently confirms the request meaning.

## Accepted rates

Both canonical NEO13 firmware contexts test the requested 32-bit value against exactly the same five decimal rates:

- 9600
- 19200
- 38400
- 57600
- 115200 bit/s

Principal OS case entry: file `0x0017F8`.

NEO Small ROM case entry: file `0x001FC2`, runtime `0x00401FC2`.

The Small-ROM entrypoint is obtained mechanically from its reverse-indexed dispatcher table, whose supported selector list contains `0x09`.

## Two-stage success handshake

For an accepted rate, both implementations first construct/send response/status `0x49` carrying the requested rate, then change the serial configuration, and finally construct/send `0x4A`, again carrying the requested rate.

The mechanically recovered success topology is therefore:

`0x09(requested_baud) -> 0x49(requested_baud) -> reconfigure serial link -> 0x4A(requested_baud)`

AlphaSync had left `0x49` unnamed. The firmware placement demonstrates that, for selector `0x09`, it is the pre-change stage of the baud transition rather than an unrelated response.

## Small-ROM hardware correlation

The NEO Small ROM performs the low-level change directly. Its `0x09` branch accesses the MC68VZ328 UART1 register block, including the baud-control register at `0xFFFFF902` (`UBAUD1`). The MC68VZ328 programmer's memory map independently identifies:

- `0xFFFFF900` — `USTCNT1`
- `0xFFFFF902` — `UBAUD1`
- `0xFFFFF904` — `URX1`
- `0xFFFFF906` — `UTX1`

The handler waits on transmitter state before changing the baud register and uses a timer/delay sequence before the final `0x4A` response. This makes the two-stage handshake mechanically meaningful: the first response is generated while the old serial configuration is still usable; the second response follows the hardware transition.

No vendor routine name is assigned.

## Principal-OS versus Small-ROM error policy

The implementations diverge on unsupported requested rates.

The principal NEO13 OS reaches its documented invalid-baud path and constructs response `0x86`, consistent with AlphaSync's `ASMESSAGE_ERROR_INVALID_BAUDRATE`.

The NEO Small ROM's initial five-rate validation branches directly to the handler epilogue when none of the five constants matches. It therefore does **not** reproduce the principal-OS `0x86` path at that point.

A distinct `0x92` response exists later inside the Small-ROM serial-configuration branch. Its exact triggering condition has not yet been source-level named and must not be relabeled as the initial unsupported-rate error merely because it lies in the same case object.

Thus:

- accepted-rate contract and `0x49 -> reconfigure -> 0x4A` topology: **CONFIRMED** in both NEO13 firmware contexts;
- principal-OS unsupported-rate response `0x86`: **CONFIRMED**;
- Small-ROM unsupported-rate direct exit: **CONFIRMED mechanically**;
- exact semantic name/trigger for the later Small-ROM `0x92`: `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`.

## Relationship to the two-generation Small-ROM work

The Small-ROM dispatcher correlation already fixes the AS3000 Small-ROM selector `0x09` entry at file `0x2976` and the NEO Small-ROM entry at `0x1FC2`. This note does not claim full two-generation `0x09` closure because the present directed pass was executed against canonical NEO13 principal OS plus canonical NEO Small ROM. A later two-generation comparison should verify the AS3000 Small-ROM UART policy and its error branch before promoting a 2-generation hardware-level invariant.

## Verification boundary

A directed local regression over canonical NEO13 principal OS SHA-256 `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0` and canonical NEO Small ROM SHA-256 `6402c40878ba28747642ce474361b1596555b2a1f137db79c7ea8bb5b35a9523` verifies the dispatcher target, five accepted baud constants in both contexts, staged `0x49`/`0x4A` responses, principal-OS `0x86`, and the Small-ROM deferred/re-entry findings carried by the same analysis run. The complete combined run passed 60/60 assertions; assertions unrelated to selector `0x09` are not counted as additional proof for this contract.

No A-line ABI entry is implied. The demonstrated A-line frontier remains A470.
