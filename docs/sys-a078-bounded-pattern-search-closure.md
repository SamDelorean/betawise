# SYS_A078 — bounded pattern search

Status: mechanically closed from primary firmware evidence.

## ABI

`SYS_A078` receives one pointer to a descriptor and returns either a pointer to the first complete match or `NULL` in `D0`.

The descriptor layout consumed by the firmware is:

| Offset | Width | Meaning |
| ---: | ---: | --- |
| `+0x00` | 32 bits | pattern length |
| `+0x04` | 32 bits | pattern pointer |
| `+0x08` | 32 bits | searched-block length |
| `+0x0C` | 32 bits | searched-block pointer |
| `+0x10` | 8 bits | comparison mode |

No vendor type name, public descriptor declaration, or stronger historical prototype has been recovered. The neutral `SYS_A078` symbol and byte layout are therefore retained instead of inventing a public structure or enum.

## Confirmed behavior

The routine searches the explicit-length block for the first complete occurrence of the explicit-length pattern.

- A zero pattern length returns `NULL`.
- A zero searched-block length returns `NULL`.
- A pattern longer than the searched block returns `NULL`.
- Otherwise, the routine evaluates `block_length - pattern_length + 1` candidate positions, including the last position at which the pattern can fit.
- A complete match returns `block_pointer + candidate_offset`.
- Exhausting the candidates returns `NULL`.
- Embedded zero bytes are ordinary data and do not terminate either input.
- The routine performs no pointer-validity checks.

## Comparison mode

The implementation proves a binary zero/nonzero mode split:

- when `mode == 0`, each byte from both inputs is transformed through syscall `A160` before comparison;
- when `mode != 0`, the input bytes are compared directly.

Historical BetaWise annotates `A160` as an international-character lowercase conversion. That annotation provides useful context, but the exact `A160` character mapping is outside this closure. Consequently, this document describes the confirmed transformation dependency without publishing an invented mode enum or broader locale contract.

## Source-first reconciliation

BetaWise keeps only the neutral `SYS_A078` entry in `os3k/syscall.c`; it does not provide a functional comment, vendor name, or public prototype for this syscall. Primary firmware evidence establishes the mechanics independently.

The routine is not equivalent to `strstr`:

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

After neutralizing only the relocated absolute address of `A160`, all three handlers are identical. Normalized handler SHA-256:

`e7efd0d593dc45989d5b1b870de13908c8ad5bf84b5c649919e0e8b0b312eb26`

Firmware bytes and ROM images remain private and are intentionally not reproduced here.

## Caller and naming evidence

No source caller or vendor function name was recovered from the indexed repository. Raw numeric occurrences are not promoted to executable callers without structural decoding. The absence of a recovered caller does not weaken the handler-derived ABI, but it prevents assigning a stronger public name or application-specific purpose.

## Refutation checks

The primary implementation and executed static regression refute these plausible alternatives:

- **NUL-terminated substring search:** false;
- **index return:** false;
- **exclusive omission of the last fitting candidate:** false;
- **multi-valued mode selector with separately demonstrated values:** unsupported; only zero versus nonzero is proven;
- **generation-dependent mechanics:** false after normalizing the single relocated helper address.

## Regression status

Static regression was **executed and passed: 57/57 checks**. It verifies the three canonical ROM identities, handler boundaries, the normalized cross-generation body, descriptor field usage, both comparison paths, all rejection guards, the last fitting candidate, pointer return construction, and embedded-zero behavior.

Regression source SHA-256:

`7008120de6d21ca5bd270ca8513ecccb13bf5430d6cf30302b28db5996da857b`

A dynamic hardware/emulator probe has not been executed; it is not required for the mechanical closure documented here.
