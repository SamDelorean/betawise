# SYS_A078 — bounded pattern search

Status: mechanically closed from primary firmware evidence and revalidated source-first.

## ABI

`SYS_A078` receives one pointer to a descriptor and returns either a pointer to the first complete match or `NULL` in `D0`.

The descriptor fields actually consumed by firmware are:

| Offset | Width | Meaning |
| ---: | ---: | --- |
| `+0x00` | 32 bits | pattern length |
| `+0x04` | 32 bits | pattern pointer |
| `+0x08` | 32 bits | searched-block length |
| `+0x0C` | 32 bits | searched-block pointer |
| `+0x10` | 8 bits | comparison mode |

No vendor type name, public descriptor declaration, or stronger historical prototype has been recovered. The neutral `SYS_A078` symbol and consumed byte layout are retained instead of inventing a public structure or enum.

A conservative C representation is therefore conceptually:

```c
const uint8_t *SYS_A078(const void *descriptor);
```

## Confirmed behavior

The routine searches the explicit-length block for the first complete occurrence of the explicit-length pattern.

- A zero pattern length returns `NULL`.
- A zero searched-block length returns `NULL`.
- A pattern longer than the searched block returns `NULL`.
- Otherwise it evaluates `block_length - pattern_length + 1` candidate positions, including the last position at which the pattern can fit.
- A complete match returns `block_pointer + candidate_offset`.
- Exhausting the candidates returns `NULL`.
- Embedded zero bytes are ordinary data and do not terminate either input.
- The routine performs no pointer-validity checks.

## Comparison mode and A160 dependency

The implementation proves a binary zero/nonzero mode split:

- when `mode != 0`, bytes are compared directly;
- when `mode == 0`, each byte from both inputs is transformed independently through `A160` before comparison.

The `A160` helper was revalidated directly rather than relying on BetaWise's historical comment. Its code converts ASCII `A..Z` to `a..z`, leaves ASCII `a..z` unchanged, and for other bytes consults a 22-entry international-character mapping table. After neutralizing its two relocated table addresses, the helper code is identical in the three canonical ROMs; normalized helper SHA-256:

`b0c8041fd06d32f77cb41e82b8d7f6e4de3fbbb059516c8e969cd160543efe1c`

The international lookup data is **not** identical across generations. AlphaSmart 3000 2005 and NEO 2005 include `0xA6` at source-table entry 13 and map it to `0xA7`; NEO 2013 instead has `0xA7` at that source entry. Consequently, `mode == 0` is ASCII case-insensitive but is not semantically identical for every high-byte value across the three generations. For example, the folded comparison `0xA6` versus `0xA7` matches in the 2005 implementations but does not match in NEO 2013.

This corrects the earlier over-broad interpretation that normalized A078 handler identity implied complete cross-generation semantic identity of folded mode.

## Source-first reconciliation

BetaWise and the independently indexed `neo-re` copy keep only the neutral `SYS_A078` entry in `syscall.c`; neither supplies a functional vendor name or public prototype. `os3k.h`/`os3k.pdf` do not expose an A078 declaration. Primary firmware therefore establishes the contract independently.

The routine is not `strstr`:

- both extents are explicit 32-bit lengths;
- embedded NUL bytes remain searchable;
- the result is a pointer into the searched block, not an index;
- the final legal candidate position is included.

## Cross-generation verification

| Platform | Handler address | `A160` helper address | Handler size |
| --- | ---: | ---: | ---: |
| AlphaSmart 3000, 2005 | `0x004CBF9E` | `0x004CEB40` | `0xAE` |
| NEO, 2005 | `0x005CBAF6` | `0x005D2C3E` | `0xAE` |
| NEO, 2013 | `0x0041C516` | `0x00423C2E` | `0xAE` |

After neutralizing only the relocated absolute address of `A160`, the three A078 handlers are identical. Normalized handler SHA-256:

`e7efd0d593dc45989d5b1b870de13908c8ad5bf84b5c649919e0e8b0b312eb26`

That code identity does not erase the documented `A160` table-data difference. Firmware bytes, table dumps, and ROM images remain private and are intentionally not reproduced here.

## Caller and xref evidence

The real A078 veneer was located structurally by the contiguous import sequence `A06C, A070, A074, A078, A07C`; executable references were then resolved using the known `MOVEA.L #disp32,A0` plus `JSR (-2,PC,A0.L)` form.

A reproducible negative scan found zero structural A078 callsites in eight representative official applets: AlphaWordPlus 2005/2012, KeyWords 2005/2012, and SpellCheck Large/Small 2005/2012. In each canonical ROM, the absolute A078 handler address also occurs only in its index-30 A-line vector slot, with no direct absolute `JSR` to that handler inside the ROM.

This is a documented negative search over that corpus, not a claim that no other applet can call A078. No vendor function name is therefore promoted beyond `SYS_A078`.

## Refutation checks

Primary code and executed regression refute or qualify these alternatives:

- **NUL-terminated substring search:** false;
- **index return:** false;
- **exclusive omission of the last fitting candidate:** false;
- **multi-valued mode selector with separately demonstrated values:** unsupported; only zero versus nonzero is proven;
- **complete generation-independent folded semantics:** false because the `A160` international table differs;
- **generation-dependent A078 control-flow mechanics:** false after normalizing the relocated helper address.

## Regression status

The original static regression executed and passed 57/57 checks. A source-first extension was then **executed and passed 91/91 checks**. The v2 regression adds direct verification of `A160`, its international mapping tables, the concrete 2005-versus-2013 folded-byte difference, the unique ROM vector reference, and the eight-applet structural xref scan.

Regression v2 source SHA-256:

`2f5f45223dcf3d0f0e378f9edfb16bc842664f1e2a6fb6900d81b46cf66a8f9c`

Regression v2 output SHA-256:

`e21d7b36cfaf6bd686780b4f310ba6d3b5d45174eb65a599bd2822bb6d75b5ce`

A dynamic hardware/emulator probe has not been executed; it is not required for the mechanical closure documented here.

## Classification

- **CONFIRMED:** one descriptor-pointer argument; consumed offsets; first-match pointer/`NULL` return; explicit lengths; final candidate included; embedded NUL behavior; zero/nonzero mode split; A160 transformation dependency; ASCII fold mechanics; generation-specific A160 lookup-table delta; normalized A078 code identity.
- **STRONG INFERENCE:** C `const uint8_t *` return/pointer terminology used here to express non-mutating observed behavior; constness itself is not encoded by the ABI.
- **PROVISIONAL:** no stronger functional/vendor symbol than `SYS_A078`.
- **UNKNOWN:** uses in official applets outside the reproducibly scanned representative corpus.
