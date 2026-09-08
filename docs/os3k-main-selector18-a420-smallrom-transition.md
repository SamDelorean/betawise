# OS3K Manager selector 0x18: main OS to Small ROM correlation

This note records only the reproducible functional contract and intentionally excludes ROM bytes, disassembly dumps, and proprietary firmware.

## Source correlation

The host-side updater flow sends Manager command `0x18` and waits for status `0x56`. The Small ROM implementation of selector `0x18` is a minimal acknowledgement handler that returns `0x56`.

## Main OS behavior (NEO 2013)

The main-OS Manager dispatcher maps selector `0x18` to a distinct handler. That handler does **not** construct response `0x56` directly. Instead, it loads ABI slot address `0x00400420` (A420) into an address register and calls it indirectly. After the call it removes two locally-pushed 32-bit arguments, does not consume the returned `D0`, and exits through the shared Manager epilogue.

The literal A420 slot address occurs only once in the canonical NEO 2013 image, in this selector-0x18 handler. This corrects an earlier direct-xref scan that reported no A420 callers because it searched direct JSR/JMP/BSR encodings only and therefore missed a literal-loaded indirect call.

The handler selects among several mechanically distinct call variants using an internal byte state and constants 3, 4, and 5. No vendor names are assigned to those state values or modes.

## Relationship to A420

A420 was previously reconstructed mechanically as a three-slot wrapper that delegates to A3DC. The selector-0x18 caller is independent firmware evidence that A420 participates in the OS-updater transition path. The exact transition mechanism below A420/A3DC is not yet demonstrated, so this relationship is classified as **strong inference**, not a vendor-level semantic name.

This caller ignores `D0`, so it does not resolve A420's contractual return type.

## Main-vs-Small distinction

The evidence supports the following separation:

- main OS selector `0x18`: invokes A420 as part of the transition/update path; it does not directly build status `0x56`;
- Small ROM selector `0x18`: acknowledges with status `0x56` once Small ROM is servicing the protocol.

The exact mechanism by which the single host request crosses the transition boundary remains unresolved. No assumption is made that the main handler itself emits the Small-ROM acknowledgement.

## Verification

A private static regression against the canonical NEO 2013 main ROM and NEO Small ROM checks dispatcher mapping, handler boundaries, the unique A420 literal reference, indirect call shape, absence of direct `0x56` construction in the main handler, and presence of the `0x56` acknowledgement in Small ROM. Final run: **19/19 PASS**.

Status: `PARCIAL_CERRADO / NO_RESUELTO` for the deep transition mechanism. No new ABI slot is promoted; A470 remains the current ABI frontier.
