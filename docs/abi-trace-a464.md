# OS3K ABI trace — A464 / index 281

Status: **mechanically reconstructed (A): no callable contract / ABI hole** for the canonical corpus.

## Canonical ROM evidence

| Platform | Vector/file slot | Raw value | Disposition |
|---|---:|---:|---|
| AlphaSmart 3000 (2005) | `file+0x02B080` | `0x0906A101` | not a pointer into the mapped ROM/runtime window |
| NEO (2005) | `file+0x0319AA` | `0x0906A101` | not a pointer into the mapped ROM/runtime window |
| NEO (2013) | `file+0x03F5C2` | `0x00000000` | null vector |

The identical 2005 raw word is still outside both demonstrated runtime windows and lies in the compact-data region. In the 2013 image, indices 278 through 281 are consecutive null entries and index 282 (`A468`) resumes with a valid code pointer.

## Official SmartApplet corpus

The canonical 41-applet corpus contains no valid `A464` stub/caller. The 16 EXTENDED applets all expose the late trap-table sequence `A450, A454, A468, A46C`, physically omitting `A464`; fourteen LEGACY applets end before this range and eleven have no A-line table.

No vendor name, arguments, return value, globals, or semantics are inferred from the shared compact word.

## Implementation consequence

Do not publish a callable prototype for `A464`. ABI tooling and emulation should model this number as unavailable in the canonical generations, not as an unresolved executable handler.

Private correlated workpapers retain canonical ROM hashes, exact slot/context evidence, caller-manifest hashes/offsets, adversarial analysis, and the executed 23/23 ROM regression.
