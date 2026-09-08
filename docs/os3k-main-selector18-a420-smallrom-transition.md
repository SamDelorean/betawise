# OS3K Manager selector 0x18: main OS to Small ROM correlation

This note records only the reproducible functional contract and intentionally excludes ROM bytes, disassembly dumps, and proprietary firmware.

## Source correlation

The host-side updater flow sends Manager command `0x18` and waits for status `0x56`. The Small ROM implementation of selector `0x18` is a minimal acknowledgement handler that returns `0x56`.

A source-first cross-check against `ioma8/neo-re` pinned at commit `732814871fe493e80ed1ab1f959d8dad174ee540` provides an important framing constraint: `alpha-core/src/protocol.rs` constructs the enter-Small-ROM request as `command(0x18, 0, 0)`. AlphaSync independently models the same transition as an enter-updater request followed by the Small-ROM `0x56` confirmation. Therefore the additional value later consumed by A420 is not a hidden field transported in the Manager `0x18` wire request.

The `neo-re` BetaWise SDK `syscall.c` also contains nominal index-generated stubs through A470, including A3DC and A420. Those generated names are useful correlation evidence, but they are not treated as proof that every nominal A-line slot is a real syscall or as proof of a functional contract.

## Main OS behavior (NEO 2013)

The main-OS Manager dispatcher maps selector `0x18` to a distinct handler. That handler does **not** construct response `0x56` directly. Instead, it loads ABI slot address `0x00400420` (A420) into an address register and calls it indirectly. After the call it removes two locally-pushed 32-bit arguments, does not consume the returned `D0`, and exits through the shared Manager epilogue.

The literal A420 slot address occurs only once in the canonical NEO 2013 image, in this selector-0x18 handler. This corrects an earlier direct-xref scan that reported no A420 callers because it searched direct JSR/JMP/BSR encodings only and therefore missed a literal-loaded indirect call.

The handler selects among several mechanically distinct call variants using an internal byte state and constants 3, 4, and 5. No vendor names are assigned to those state values or modes.

## Corrected A420 call-frame geometry

Earlier notes described A420 mechanically as receiving three stack slots. The selector-0x18 caller proves that this wording needs refinement.

The Manager dispatcher creates a local stack frame and reaches selector cases through an intra-function indexed jump. Selector `0x18` explicitly pushes only two 32-bit values before calling the A420 veneer. The A-line dispatcher removes the exception frame before jumping to the resolved A420 handler. At A420 entry, after A420 saves one address register, its access at `0x10(SP)` resolves exactly to the Manager dispatcher's stack pointer immediately before selector `0x18` pushed those two explicit values. In other words, A420 obtains the first longword of the Manager dispatcher's local frame, not a third wire parameter and not an ordinary third C argument explicitly supplied by selector `0x18`.

A420 then treats that inherited longword as a pointer and consumes at least four 32-bit fields from the pointed object before delegating further. The mechanical identity can therefore be recorded provisionally as `manager_dispatcher_local0_context_ptr`. The producer and semantic type of that object are still unresolved; no vendor name is assigned.

This correction refutes the hypothesis that the extra A420 context comes from the `0x18` Manager payload.

## Relationship to A420 and A3DC

The selector-0x18 caller is independent firmware evidence that A420 participates in the OS-updater transition path. A420 delegates into A3DC, but the exact transition mechanism below A420/A3DC is not yet demonstrated, so this relationship remains classified as **strong inference**, not a vendor-level semantic name.

The caller ignores `D0`, so it does not resolve A420's contractual return type.

## Main-vs-Small distinction

The evidence supports the following separation:

- main OS selector `0x18`: invokes A420 as part of the transition/update path; it does not directly build status `0x56`;
- Small ROM selector `0x18`: acknowledges with status `0x56` once Small ROM is servicing the protocol.

The exact mechanism by which the single host request crosses the transition boundary remains unresolved. No assumption is made that the main handler itself emits the Small-ROM acknowledgement.

## Verification

A private static regression against the canonical NEO 2013 image verifies the Manager dispatcher local-frame geometry, indexed selector dispatch, the selector-0x18 two-argument call shape, A-line exception-frame removal, A420's inherited-context access, and the four-field dereference pattern. Final run for this correction: **17/17 PASS**. Earlier transition checks covering main/Small-ROM separation remain independently archived.

Status: `PARCIAL_CERRADO / NO_RESUELTO` for the producer/type of the inherited dispatcher context and the deep transition mechanism. No new ABI slot is promoted; A470 remains the current ABI frontier.
