# OS3K Small ROM selectors `0x09`, `0x0A`, and `0x19`

This note documents Manager/update-protocol handlers in the AlphaSmart 3000 and NEO Small ROMs. These are **not** A-line syscalls and do not extend the demonstrated A-line ABI frontier.

## Source-first identity of `0x09`

AlphaSync defines request `0x09` as `SET_BAUDRATE` with a 32-bit baud-rate argument and identifies response `0x4A` as the baud-rate response. neotools carries the same request/response names and packet comments, but its own README states that its device driver was ported from AlphaSync. It is therefore useful corroboration of implementation lineage, **not an independent source** for the protocol identity.

Both codebases leave response `0x49` unnamed. This note therefore does not invent a vendor name for `0x49`.

Direct comparison of the historical AlphaSmart 3000 Small ROM from AlphaSmart Manager 2.3 and the NEO Small ROM from NEO Manager 3.9.3 confirms that selector `0x09` accepts exactly five rates: 9600, 19200, 38400, 57600, and 115200 baud. Accepted values produce a preliminary `0x49`, serial/timer reconfiguration, a serial-status wait, and response `0x4A`; both Small ROM generations also preserve the same later `0x92` path.

A fresh canonical two-generation revalidation further shows that the complete `0x09` handlers are both exactly `0x108` bytes and differ in only five bytes. Those five differences occur at the same relative positions in the per-rate programming-parameter block, one for each supported rate. The comparison/control prefix, the complete tail, the `0x49`/`0x4A`/`0x92` response geometry, and the relevant DragonBall serial/timer MMIO accesses are otherwise identical. Neither Small ROM handler contains the fixed `0x86` unsupported-rate response used by the NEO principal-OS implementation.

The request identity is therefore source-grounded and the argument/rate/register mechanics are directly confirmed across both Small-ROM generations. The exact protocol name of `0x49` and the precise trigger/vendor meaning of `0x92` remain unresolved.

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

## Shared deferred restart/re-entry primitive

AlphaSync provides an independent source anchor for request `0x08`: it names the request `RESTART`, describes it as causing the device to reset and restart as a HID device, and identifies response `0x52`. The neighboring request `0x07` remains historically unnamed, although AlphaSync records that it is used while writing an applet and receives response `0x48`.

The Small-ROM firmware makes the relationship between the two requests explicit. In both the AlphaSmart 3000 and NEO generations, requests `0x08` and `0x07` set the **same deferred-transition flag** before returning their distinct responses. The transport path later tests and clears that flag before invoking one shared low-level helper.

That helper is a bounded `0x34`-byte no-return object at runtime `0x0040051E` in the AlphaSmart 3000 Small ROM and `0x0040059E` in the NEO Small ROM. Its caller-visible mechanics are homologous across generations: it quiesces/reprograms low-level state, reloads the generation-specific stack pointer, clears the frame/base register used by the Small ROM, raises the status-register interrupt mask, and terminates by jumping to a generation-specific re-entry entrypoint rather than returning. After normalizing the stack immediate and final re-entry target, the two objects are otherwise identical.

This closes the helper as the **shared deferred restart/re-entry primitive** used by the known `0x08` RESTART path. It also closes the mechanical effect of `0x07`: that request arms the same deferred restart/re-entry transition after its `0x48` response. The historical/vendor name and higher-level policy for `0x07` remain **UNRESOLVED / INSUFFICIENT EVIDENCE**; no name is invented here.

## Verification boundary

The earlier multi-selector Small-ROM regression passes **45/45 assertions**. A new direct AS3000↔NEO `0x09` regression passes **42/42 assertions**, covering both canonical hashes, exact `0x108` handler boundaries, the five accepted rates at matching offsets, complete-handler difference accounting, preserved response geometry, MMIO references, and absence of a fixed `0x86` Small-ROM response. A private two-generation regression focused on selectors `0x0A` and `0x19` passes **39/39 assertions**. A separate two-generation regression for the shared deferred restart/re-entry primitive also passes **39/39 assertions**.

Status:

- selector `0x09` / historical `SET_BAUDRATE`: **CONFIRMED**;
- accepted baud-rate set and direct AS3000↔NEO Small-ROM mechanics: **CONFIRMED**;
- complete AS3000↔NEO handler homology except five per-rate programming-parameter bytes: **CONFIRMED**;
- exact semantic name of response `0x49`: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- exact trigger/vendor meaning of the preserved `0x92` path: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x19` transport callback-gate mechanics: **CONFIRMED**;
- selector `0x19` vendor name / AlphaHub identity: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x0A` dynamic last-response-code replay mechanics: **CONFIRMED**;
- selector `0x0A` vendor name / intended policy: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- fixed-`0x90` interpretation for selector `0x0A`: **REFUTED**;
- shared AS3000 `0x0040051E` / NEO `0x0040059E` deferred restart/re-entry helper: **CONFIRMED**;
- request `0x07` deferred restart/re-entry effect: **CONFIRMED**, vendor name/policy **UNRESOLVED / INSUFFICIENT EVIDENCE**.

No ROM bytes or extended disassembly are published here. No A-line ABI promotion is implied; the demonstrated A-line frontier remains A470.
