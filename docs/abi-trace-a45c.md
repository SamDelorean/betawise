# OS3K ABI trace — A45C / index 279

Status: **MECANICA_CERRADA A / SOURCE_FIRST / NO_CONTRATO_CALLABLE**.

## Source correlation

The recovered BetaWise/ioma8 syscall mapping omits index 279 entirely: it maps A454/index277 and resumes at A468/index282. No `SYS_A45C` definition or independent prototype was recovered. This negative source evidence agrees with all three canonical ROMs and the official SmartApplet corpus.

## Canonical ROM evidence

| Platform | Vector/file slot | Raw value | Disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | `file+0x02B078` | `0x06080304` | outside the mapped ROM/runtime window; non-callable |
| NEO (2005) | `file+0x0319A2` | `0x03040102` | outside the mapped ROM/runtime window; non-callable |
| NEO (2013) | `file+0x03F5BA` | `0x00000000` | null vector |

The 2005 images are already in a compact-data sequence rather than a valid pointer run. In NEO 2013, indices 278–281 are four consecutive null entries; index 282 (`A468`) resumes with a valid code pointer.

## Official SmartApplet corpus

The canonical 41-applet sweep is negative for A45C. All 16 EXTENDED applets expose the late callable sequence `A450, A454, A468, A46C` and physically omit A458–A464. Fourteen LEGACY applets end before this range and eleven have no A-line table.

The existing A45C-specific ROM regression is `23/23 PASS`; the shared 41/41 caller/stub-negative manifest was revalidated at the adjacent A458 closure. Dynamic execution is not applicable because no executable target exists.

## Mechanical disposition

A45C/index279 is an **ABI hole / non-callable slot** in the canonical corpus. No handler body, arguments, return value, helper/global contract, semantic function name, or callable C prototype is asserted.

For emulation and ABI tooling, model this slot as unavailable for these canonical generations. This conclusion is corpus-bounded and does not claim the numeric trap was never reused in an unavailable build.
