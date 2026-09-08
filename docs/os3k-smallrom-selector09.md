# OS3K Small ROM selectors `0x09`, `0x0A`, and `0x19`

This note documents Manager/update-protocol handlers in the AlphaSmart 3000 and NEO Small ROMs. These are **not** A-line syscalls and do not extend the demonstrated A-line ABI frontier.

## Source-first identity of `0x09`

AlphaSync defines request `0x09` as `SET_BAUDRATE` with a 32-bit baud-rate argument and identifies response `0x4A` as the baud-rate response. neotools carries the same request/response names and packet comments, but its own README states that its device driver was ported from AlphaSync. It is therefore useful corroboration of implementation lineage, **not an independent source** for the protocol identity.

Both codebases leave response `0x49` unnamed. This note therefore does not invent a vendor name for `0x49`.

Direct comparison of the historical AlphaSmart 3000 Small ROM from AlphaSmart Manager 2.3 and the NEO Small ROM from NEO Manager 3.9.3 confirms that selector `0x09` accepts exactly five rates: 9600, 19200, 38400, 57600, and 115200 baud. Accepted values produce a preliminary `0x49`, serial/timer reconfiguration, a serial-status wait, and response `0x4A`; invalid values take an error path containing `0x92`.

The request identity is therefore source-grounded and the argument/rate/register mechanics are independently firmware-confirmed. The exact protocol name of `0x49` and the vendor meaning of `0x92` remain unresolved.

## Selector `0x19`: transport callback gate

AlphaSync leaves request `0x19` unknown and speculates that it may be AlphaHub-specific. neotools reproduces that comment through the AlphaSync-derived driver lineage, so the AlphaHub suggestion is **not independent evidence** and remains unsupported as an identity claim.

Direct firmware verification nevertheless closes the mechanics in both Small-ROM generations:

- the request consumes a word argument;
- zero clears a byte gate and nonzero sets it;
- initialization sets that gate to enabled;
- the handler emits response `0x57`;
- exhaustive direct-reference accounting finds one operational consumer outside initialization/handler writes;
- that consumer tests the gate before loading and invoking the active receive/dispatch callback used by the Small-ROM transport path.

Therefore selector `0x19` **enables or inhibits invocation of the active transport receive/dispatch callback**. That caller-visible mechanical role is confirmed in both generations. A vendor request name, product-level purpose, and any AlphaHub association remain **UNRESOLVED / INSUFFICIENT EVIDENCE**.

## Selector `0x0A`: dynamic response-code replay

AlphaSync leaves request `0x0A` unknown. neotools repeats the same unknown/empirical-`0x90` observation through the same source lineage, so the reported `0x90` is not an independent protocol definition.

Direct firmware analysis resolves why the handler itself contains no fixed `0x90`:

- the common response builder records the response-code byte every time it emits a response;
- the recorded byte has exactly one other direct consumer: selector `0x0A`;
- `0x0A` reads that stored response code, supplies zero for the other response fields, and re-enters the common response-builder call path;
- the mechanism is homologous in the AlphaSmart 3000 and NEO Small ROMs.

Thus `0x0A` **re-emits the most recently recorded response code with zero argument/trailing fields**. A tool can consequently observe `0x90` when `0x90` was the relevant prior response, but `0x90` is not a fixed literal or invariant contract of selector `0x0A`.

The firmware mechanics are confirmed; the historical vendor request name and intended policy/use remain **UNRESOLVED / INSUFFICIENT EVIDENCE**. This note intentionally avoids inventing a source-level name such as “resend” or “retry.”

## Verification boundary

The earlier baud-rate regression passes **45/45 assertions**. A second private two-generation regression focused on selectors `0x0A` and `0x19` passes **39/39 assertions**, covering canonical Small-ROM hashes, handler homology, gate initialization/reference accounting, callback gating, response-state reference accounting, common response-builder linkage, and the absence of any fixed `0x90` contract in `0x0A`.

Status:

- selector `0x09` / historical `SET_BAUDRATE`: **CONFIRMED**;
- accepted baud-rate set and two-generation mechanics: **CONFIRMED**;
- exact semantic name of response `0x49`: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x19` transport callback-gate mechanics: **CONFIRMED**;
- selector `0x19` vendor name / AlphaHub identity: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x0A` dynamic last-response-code replay mechanics: **CONFIRMED**;
- selector `0x0A` vendor name / intended policy: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- fixed-`0x90` interpretation for selector `0x0A`: **REFUTED**.

No ROM bytes or extended disassembly are published here.
