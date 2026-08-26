# System 3 file-space accounting — A1AC, A1B0 and A1BC

This note closes the mechanical behavior of three neighboring System 3 / OS3K File API traps. The implementations were compared directly in AlphaSmart 3000 System 3 (Nov 2005), NEO System 3 (Nov 2005), and NEO System 3 (Jul 2013).

The ABI mechanics below are confidence **A**. Original public names have **not** been recovered, so these traps intentionally remain `SYS_A1AC`, `SYS_A1B0`, and `SYS_A1BC` in the SDK rather than receiving invented names.

## Stable handler locations

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | Handler length |
| --- | ---: | ---: | ---: | ---: |
| A1AC | `0x4E16D8` | `0x5E4034` | `0x43A436` | `0xC8` |
| A1B0 | `0x4E17A0` | `0x5E40FC` | `0x43A4FE` | `0x40` |
| A1BC | `0x4E19E4` | `0x5E4340` | `0x43A742` | `0x36` |

The A1AC implementations have the same control flow and descriptor offsets; 193 of 200 bytes are identical across all three ROMs, with differences confined to relocated globals/calls and the source-line trace constant. A1B0 and A1BC are likewise structurally equivalent across the three firmware generations.

## A1B0 — unused capacity already assigned to one descriptor

Mechanical research signature:

```c
uint32_t SYS_A1B0(uint16_t file_id);
```

The handler resolves `file_id` through the same 16-bit descriptor resolver used by A1C4/A1C8. If resolution fails it returns `0`. On success it returns:

```text
descriptor->max_size - descriptor->current_size
```

using descriptor offsets `+0x0C` and `+0x04` respectively.

This value is best described mechanically as **unused capacity inside the descriptor's current maximum allocation**. It should not yet be called total system free space: A1AC accounts for additional global/reclaimable storage separately.

Official caller evidence strengthens the token interpretation. AlphaWord Plus contains thirteen A1B0 callers and AlphaQuiz contains two. Callers promote a 16-bit file token into the 32-bit ABI stack slot before invoking the trap. AlphaWord Plus also calls A1B0 with reserved token `0x00CB`, proving that the operation applies to the clipboard descriptor as well as ordinary file descriptors.

## A1BC — descriptor maximum capacity

Mechanical research signature:

```c
uint32_t SYS_A1BC(uint16_t file_id);
```

A1BC uses the same resolver and returns `0` when the token does not resolve. On success it returns:

```text
descriptor->max_size
```

from descriptor offset `+0x0C`.

No direct caller of A1BC was found in the official AlphaWord Plus, AlphaQuiz, KeyWords, Calculator, or ControlPanel binaries currently in the project. This negative caller result does not weaken the handler-level ABI: the three ROM implementations are explicit and equivalent.

## Exact relation with A1B4

For an **ordinary resolvable file token**, A1B4 returns `descriptor->current_size`. Therefore the following identity is a firmware-backed regression invariant:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

or, expressed by fields:

```text
current_size + (max_size - current_size) == max_size
```

This invariant applies only to ordinary token resolution. A1B4 also accepts special selectors `0xFC`..`0xFF`, whose accounting semantics are documented separately and must not be substituted into this identity.

## A1AC — global/reclaimable storage accounting

Mechanical research signature:

```c
uint32_t SYS_A1AC(void);
```

A1AC takes no public arguments and returns a 32-bit character/storage count. It is not a simple sum of `max_size-current_size`. The handler implements allocation accounting in **512-byte (`0x200`) units**, using mask `0xFFFFFE00` to round reclaimable regions down to a 512-byte boundary.

The NEO 2013 implementation makes the algorithm particularly clear. The same logic appears in the AS3000 and NEO 2005 handlers.

### Clipboard contribution

For the dedicated clipboard descriptor, it computes:

```text
reclaimable = (max_size - previous_or_recoverable_size) & ~0x1FF
if reclaimable <= 0x200:
    reclaimable = 0
```

and begins the accumulated result with this contribution.

### Unallocated tail

It then adds the raw gap between the end of the last allocated descriptor storage region and the global end-of-file-storage pointer. In NEO 2013 the relevant runtime globals are the descriptor-table/end pointer near `0x34D6` and the storage-end pointer at `0x35DE`.

### Descriptor reclaimable contribution

For a descriptor, A1AC first chooses the larger of:

```text
previous_or_recoverable_size (+0x08)
accounting/reservation_threshold (+0x10)
```

It subtracts that watermark from `max_size`, rounds the result down to a 512-byte boundary, and counts it only when the rounded amount is greater than one 512-byte block.

When an active descriptor exists, the routine applies this contribution to that active descriptor. When no descriptor is active, it walks the full `0x48`-byte descriptor table and applies the same calculation to each entry, excluding the separately-accounted clipboard descriptor.

The result is therefore a **global allocator/reclaimable-space metric**, not merely a file's immediate unused capacity.

## AlphaWord Plus caller correlation

The official AlphaWord Plus binary contains two A1AC callers and thirteen A1B0 callers. The two A1AC calls occur in the pair of routines that build the character/page storage-status displays; the same binary contains the visible labels `Characters used`, `Characters available`, `Pages used`, and `Pages available` for separate file and `System` columns.

This caller/UI correlation supports the interpretation that A1AC supplies system-level available capacity while A1B0 participates in per-file capacity calculations. It is supporting evidence, not a recovered source-level function name.

## Historical-source result

The original AS3000 `FileModule.c` / `FileModule.h` from 2000 were searched for matching free-space/available-space APIs. No public function corresponding directly to A1AC, A1B0, or A1BC exists there. The early implementation used compile-time fixed per-file capacities, while later System 3 introduced dynamic allocation/accounting behavior.

Consequently, names such as `FileGetFreeSpace`, `FileGetAvailableSpace`, or `FileGetMaxFileSize` would currently be modern descriptive inventions rather than recovered original API names. They are deliberately not promoted into `os3k.h` yet.

## Regression requirements

Once these calls are exposed to the emulator regression harness, the minimum checks are:

1. ordinary file token: A1B4 + A1B0 equals A1BC;
2. unresolved token: A1B0 and A1BC return zero;
3. clipboard token `0x00CB`: A1B0 returns `max_size-current_size` for the clipboard descriptor;
4. A1AC result changes consistently when allocator slack crosses 512-byte boundaries;
5. A1AC does not count a rounded reclaimable region of `0x200` bytes or less;
6. A1AC excludes the clipboard from the general descriptor-table loop because it is accounted separately.

## Closure status

- **A1B0 mechanics:** closed, confidence A; public name pending.
- **A1BC mechanics:** closed, confidence A; public name pending.
- **A1AC mechanics:** closed, confidence A; public name pending.
- No `os3k.h` promotion is made in this milestone because recovering or deliberately choosing the public names is a separate naming decision from the now-closed ABI behavior.
