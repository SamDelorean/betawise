# OS3K ABI trace — A460 / index 280

Status: **mechanically reconstructed (A): no callable contract / ABI hole** for the canonical corpus.

## Canonical ROM evidence

| Platform | Vector/file slot | Raw value | Disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | `file+0x02B07C` | `0x07090501` | not a pointer into the mapped ROM/runtime window |
| NEO (2005) | `file+0x0319A6` | `0x03040501` | not a pointer into the mapped ROM/runtime window |
| NEO (2013) | `file+0x03F5BE` | `0x00000000` | null vector |

The 2005 images are in the same compact-data region following the preceding callable range; both raw values are outside the demonstrated mapped ROM windows. In the 2013 image, indices 278 through 281 are consecutive null entries and index 282 (`A468`) resumes with a valid code pointer.

## Official SmartApplet corpus

The canonical 41-applet corpus contains no valid `A460` stub/caller. The 16 EXTENDED applets all expose the late trap-table sequence `A450, A454, A468, A46C`, physically omitting `A460`; fourteen LEGACY applets end before this range and eleven have no A-line table.

This is an availability statement only. It does not assign a vendor name, arguments, return value, globals, or hidden semantics to the number.

## Implementation consequence

Do not publish a callable prototype for `A460`. ABI tooling and emulation should model this number as unavailable in the canonical generations rather than as an unresolved executable handler.

Private correlated workpapers retain the canonical ROM hashes, exact slot/context evidence, caller-manifest hashes/offsets, adversarial analysis, and the executed 23/23 ROM regression.
