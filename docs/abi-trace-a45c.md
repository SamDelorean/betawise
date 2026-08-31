# OS3K ABI trace — A45C / index 279

Status: **mechanically reconstructed (A): no callable contract / ABI hole** for the canonical corpus.

## Canonical ROM evidence

| Platform | Vector/file slot | Raw value | Disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | `file+0x02B078` | `0x06080304` | not a pointer into the mapped ROM/runtime window |
| NEO (2005) | `file+0x0319A2` | `0x03040102` | not a pointer into the mapped ROM/runtime window |
| NEO (2013) | `file+0x03F5BA` | `0x00000000` | null vector |

The 2005 images enter a compact-data region after the preceding callable range; the values above are outside the demonstrated mapped ROM windows. In the 2013 image, indices 278 through 281 are four consecutive null entries and index 282 (`A468`) resumes with a valid code pointer.

## Official SmartApplet corpus

The canonical 41-applet corpus contains no valid `A45C` stub/caller. The 16 EXTENDED applets all expose the same late trap-table sequence `A450, A454, A468, A46C`, physically omitting `A45C` (as well as the adjacent holes). Fourteen LEGACY applets end before this range and eleven have no A-line table.

This is an availability statement only: it does **not** assign a vendor name, arguments, return value, globals, or hidden semantics to the number.

## Implementation consequence

Do not publish a callable prototype for `A45C`. For emulation and ABI tooling, model it as unavailable for the canonical generations rather than as an unresolved executable handler.

Private correlated workpapers retain ROM hashes, exact contextual words, caller-manifest hashes/offsets, adversarial analysis, and regression evidence.
