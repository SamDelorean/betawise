# OS3K Small ROM common block engine across generations

This note compares the historical AlphaSmart 3000 Small ROM distributed with AlphaSmart Manager 2.3 against the NEO Small ROM distributed with NEO Manager 3.9.3. The comparison is source-first and then verified directly in both firmware images.

These are Manager/update-protocol handlers, not A-line syscalls.

## Images

| Image | Size | Internal identity | SHA-256 |
| --- | ---: | --- | --- |
| AlphaSmart 3000 Small ROM | `0x3A96` | `OS 3000 Small ROM` | `479adbe2f873da053046a749f95d2c86eeada23df442237a613b09e9780fd874` |
| NEO Small ROM | `0x581A` | `OS 3KNeo Small ROM` | `6402c40878ba28747642ce474361b1596555b2a1f137db79c7ea8bb5b35a9523` |

## Dispatcher invariance

Both images contain the same eleven-command selector list:

```text
00 02 07 08 09 0A 0B 16 17 18 19
```

Both use the same reverse-indexed displacement-table architecture. Direct resolution gives:

| Selector | AS3000 Small ROM | NEO Small ROM |
| --- | ---: | ---: |
| `0x00` | `0x270A` | `0x1D1A` |
| `0x02` | `0x275A` | `0x1D6A` |
| `0x07` | `0x2940` | `0x1F8C` |
| `0x08` | `0x292C` | `0x1F78` |
| `0x09` | `0x2976` | `0x1FC2` |
| `0x0A` | `0x2792` | `0x1DA2` |
| `0x0B` | `0x27A4` | `0x1DB4` |
| `0x16` | `0x2600` | `0x1C10` |
| `0x17` | `0x2614` | `0x1C24` |
| `0x18` | `0x25F4` | `0x1C04` |
| `0x19` | `0x2954` | `0x1FA0` |

The addresses above are file offsets inside each Small ROM. They establish physical object boundaries only; they are not ABI slots.

## `0x02` and raw receive are mechanically homologous

The `0x02` handler is `0x38` bytes in both generations. In each image it clears the current checksum state, latches the expected checksum and block length, computes an end cursor, installs a dedicated raw receiver, and prepares response `0x42`.

The dedicated raw callbacks are also the same size (`0x9C` bytes):

- AS3000 runtime `0x00402A7E`
- NEO runtime `0x004020CA`

After relocation of globals and callback addresses, the same central machine is preserved: byte copy, cursor advance, 16-bit additive checksum, completion test, success response `0x43`, and checksum-failure rewind with response `0x8E`.

This is strong generational evidence that `0x02`/raw is a reusable block-receive primitive rather than a feature-specific command.

## Destination setup is also homologous

Several setup/finalization handlers retain exact physical geometry across both images:

| Selector | AS3000 size | NEO size | Host-visible result |
| --- | ---: | ---: | --- |
| `0x16` | `0x14` | `0x14` | `0x54` |
| `0x17` | `0xF6` | `0xF6` | `0x55` |
| `0x18` | `0x0C` | `0x0C` | `0x56` |
| `0x07` | `0x14` | `0x14` | deferred-finalize flag + `0x48` |

The matching geometry is not used as the sole semantic proof; it corroborates the independently observed host protocol and direct handler behavior.

## `0x0B` preserves the contract and has one isolated NEO extension

`0x0B` preserves the same mapped-block commit/program contract but differs physically:

- AS3000 body: `0x188` bytes
- NEO body: `0x1C4` bytes
- net growth: `0x3C` bytes

A fresh word-aligned comparison of canonical Small ROMs resolves that growth completely. The entire `0x3C` delta is one contiguous NEO-only pre-programming path. It indexes the current segment entry, compares that segment base with `0x005FFC00`, and bypasses the complete extra path when the comparison does not match. When it does match, the firmware resets per-transfer state, rearms the staging cursor, prepares response `0x47` with the common response builder, and then rejoins the ordinary programming path.

Removing exactly that 60-byte path from the structural comparison makes the NEO `0x0B` body the same `0x188` length as AS3000. The remaining differences are operand relocations and control-flow displacement changes rather than another added code block: 19 relocated global operands, five helper-target operands, and two branch displacements.

This directly refutes the earlier possible explanation that NEO's larger handler represents introduction of the segment mapper itself. The mapped commit/program skeleton is already homologous; NEO adds a segment-specific policy path on top of it.

The higher-level product reason why the segment at `0x005FFC00` receives special handling is not independently demonstrated. It therefore remains **UNRESOLVED / INSUFFICIENT EVIDENCE** and should not be labeled as a bank, fallback, recovery image, or similar architecture without additional evidence.

## Architectural result

The combined source and firmware evidence supports the following cross-generation model:

```text
DESTINATION_SETUP
    -> COMMON_BLOCK_ENGINE
       0x02 announce/arm(len,sum16)
       raw bytes -> validate sum16
       0x43 success / 0x8E rewind on checksum failure
       0x0B mapped program/commit
          [NEO: optional segment-specific pre-path]
       0x47
    -> 0x07 deferred finalization
       0x48
```

The same transport engine survived the transition from the AlphaSmart 3000 Small ROM to the later NEO Small ROM. The generational change now isolated in `0x0B` is an added segment-specific path, not a replacement of the common transfer engine.

## Verification boundary

The original private two-image regression passes **74/74 assertions** for dispatcher/engine homology. A second private regression focused on the `0x0B` delta passes **45/45 assertions** against freshly verified canonical Small ROM images. It checks image hashes, handler boundaries and sizes, the single `0x3C` insertion model, the unique NEO segment-key comparison, the exact common-continuation bypass, retained response/staging behavior, and the absence of another added opcode block outside the isolated path.

Status:

- dispatcher namespace/geometry across AS3000 and NEO: **CONFIRMED**;
- common `0x02`/raw block receive state machine: **CONFIRMED**;
- `0x16/0x17/0x18/0x07` structural homology: **CONFIRMED**;
- `0x0B` common commit/program contract: **CONFIRMED**;
- complete physical explanation of the `0x188 -> 0x1C4` `0x0B` growth: **CONFIRMED**;
- NEO-only segment-specific pre-programming path keyed by `0x005FFC00`: **CONFIRMED mechanically**;
- higher-level product purpose of that special segment handling: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- deferred restart/re-entry helper used after `0x07`: **CONFIRMED mechanically across both Small ROM generations**, while the vendor name of `0x07` remains unresolved.

No ROM image or large disassembly is published. No A-line ABI promotion is implied; the demonstrated A-line frontier remains A470.