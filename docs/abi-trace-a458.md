# SYS_A458 / index 278 — ABI trace

Status: **MECANICA_CERRADA A / SOURCE_FIRST / NO_CONTRATO_CALLABLE**.

## Source correlation

The recovered BetaWise/ioma8 syscall mapping contains `SYS_A454` at index 277 and resumes with `SYS_A468` at index 282. There is no recovered `SYS_A458` definition at index 278. This negative source evidence independently agrees with the canonical firmware and SmartApplet corpus.

## Availability

- AlphaSmart 3000 (2005): slot 278 is `0x04020205`, outside the mapped ROM runtime window; it is not a callable pointer.
- NEO (2005): slot 278 is `0x04020002`, likewise outside the mapped ROM runtime window.
- NEO (2013): slot 278 is `0x00000000`. Slots 279–281 are also null; a valid pointer resumes at index 282 (`A468`).

The 2005 values occur inside a compact data-like sequence rather than a valid pointer run. The NEO 2013 four-slot null gap provides independent generational corroboration that A458–A464 are not callable entries in the canonical image.

## Official SmartApplet corpus

The canonical 41-SmartApplet sweep is negative for A458. All 16 EXTENDED applets carry the late callable sequence `A450, A454, A468, A46C` and omit A458–A464. Fourteen legacy table-bearing applets terminate before this range, and eleven corpus applets have no A-line table.

Therefore the canonical corpus contains no valid A458 stub or executable caller.

## Mechanical disposition

A458/index278 is an **ABI hole / non-callable slot** in the canonical evidence set. There is no handler body, stack/register ABI, return contract, helper/global contract, or callable C prototype to reconstruct.

Earlier static regression: `47/47 PASS`. Dynamic execution is not applicable because there is no callable handler target.

This conclusion is deliberately corpus-bounded: it does not claim that the numeric trap was never used in an unavailable vendor build.

## Publication boundary

No vendor function name, signature, enum, synthetic implementation, ROM byte dump, or fabricated behavior is asserted.
