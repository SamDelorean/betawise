# SYS_A208 / A208 — clipboard paste primitive

Status: mechanically and semantically closed from the three canonical System 3 generations. A historical AlphaSmart symbol, `FilePasteClipboard(void)`, is recovered for the earlier File-module lineage, but an ABI-identical modern System 3 symbol has not been recovered. The modern neutral name therefore remains `SYS_A208`.

## Contract

```c
int32_t SYS_A208(uint8_t write_mode, uint8_t read_variant);
```

A208 pastes the dedicated System 3 clipboard into the active file by delegating to the A198 file-write family.

The second byte is tested exactly against `3`:

- `read_variant == 3`: use the private A198 transfer entry at `A198 + 0x1BA`, reconstructed as the control-code-aware/filtered source path;
- any other value: use public `A198 / FileWriteBuffer`.

In both branches, A208 passes active token `0`, clipboard storage, clipboard current size, and the first argument `write_mode` unchanged. The selected writer result is returned.

The write-mode semantics are inherited from A198:

- `1`: append at EOF;
- `2`: insert at cursor;
- `3`: overwrite at cursor;
- other values reach the underlying raw `-5` mode error.

## Naming lineage

Early AlphaSmart File-module material contains the genuine historical declaration `FilePasteClipboard(void)`. This is strong genealogical evidence for the paste concept, but the modern handler takes two explicit selector bytes and no primary source or symbol table has yet proved that the old symbol survived unchanged into the System 3 ABI. Therefore:

- **historical name confirmed:** `FilePasteClipboard(void)` for the earlier lineage;
- **modern vendor name unknown:** retain `SYS_A208` rather than silently upgrading the historical name.

## Firmware evidence

Canonical public handlers:

| ROM | entry | length |
| --- | ---: | ---: |
| AlphaSmart 3000 System 3 (2005) | `0x004E0B24` | `0x66` |
| NEO System 3 (2005) | `0x005E3480` | `0x66` |
| NEO System 3 (2013) | `0x0043987E` | `0x66` |

Direct revalidation confirms the exact `read_variant == 3` comparison and the two writer targets:

| ROM | public A198 | filtered/private entry |
| --- | ---: | ---: |
| AS3000 | `0x004E1196` | `0x004E1350` |
| NEO 2005 | `0x005E3AF2` | `0x005E3CAC` |
| NEO 2013 | `0x00439EF4` | `0x0043A0AE` |

The filtered entry is exactly `A198 + 0x1BA` in all three generations. Inside A208 the filtered call occurs at handler offset `+0x24`, while the public A198 call occurs at `+0x44`.

No direct absolute JSR to the public A208 handler was found in the three complete firmware images (`0/0/0`). Prior official SmartApplet correlation found an AlphaQuiz A-line call with `(3,3)`, consistent with overwrite mode plus the filtered/control-code-aware path.

## Refutation

- `read_variant` is not treated as a generic boolean: only the exact value `3` chooses the alternate path.
- A208 is not a standalone paste implementation; it delegates to the public or private A198 writer entry.
- The historical `FilePasteClipboard` name is not automatically asserted as the modern ABI name because the signature evolved and no modern primary symbol has been recovered.

## Confidence

- **CONFIRMED:** two-byte ABI; exact `read_variant == 3` selector; writer targets; passthrough `write_mode`; passthrough writer return; equivalent semantics across all three canonical generations.
- **STRONG INFERENCE:** descriptive role “paste clipboard into active file.”
- **HISTORICAL NAME CONFIRMED:** `FilePasteClipboard(void)` for the earlier File-module lineage.
- **UNKNOWN:** exact modern vendor symbol and original enum labels for `write_mode` and `read_variant`.

Static source-first revalidation: **30/30 PASS**. Dynamic/emulator regression remains **SPECIFIED / NOT EXECUTED**, covering write modes 1/2/3, invalid mode `-5`, and raw versus filtered handling of known control sequences.
