# System 3 file-space accounting — A1AC, A1B0 and A1BC

This note closes the mechanical behavior of three neighboring System 3 / OS3K
File API traps. The implementations were compared directly in AlphaSmart 3000
System 3 (Nov 2005), NEO System 3 (Nov 2005), and NEO System 3 (Jul 2013).

The ABI mechanics below are confidence **A**. Original public names have **not**
been recovered, so these traps intentionally remain `SYS_A1AC`, `SYS_A1B0`, and
`SYS_A1BC` rather than receiving invented names.

## Stable handler locations

| Trap | AS3000 2005 | NEO 2005 | NEO 2013 | Handler length |
| --- | ---: | ---: | ---: | ---: |
| A1AC | `0x4E16D8` | `0x5E4034` | `0x43A436` | `0xC8` |
| A1B0 | `0x4E17A0` | `0x5E40FC` | `0x43A4FE` | `0x40` |
| A1BC | `0x4E19E4` | `0x5E4340` | `0x43A742` | `0x36` |

The A1AC implementations have the same control flow and descriptor offsets; 193
of 200 bytes are identical across all three ROMs, with differences confined to
relocated globals/calls and the source-line trace constant. A1B0 and A1BC are
likewise structurally equivalent.

## Descriptor +0x10: min_size correction

Later analysis of A1E8 closes descriptor offset `+0x10` as the file's **minimum
allocation / `min_size`**. A1E8 receives an explicit `min_size` argument, stores
it at `+0x10`, and calculates new allocation as:

```text
effective_max = max(requested_max_size, min_size, 0x200)
```

The System 3 ROM additionally contains the diagnostic string:

```text
File %d Min Size overflow
```

This supersedes the earlier neutral project label
`accounting/reservation_threshold`. The accounting mechanics previously derived
from A1AC remain correct; the field now has a stronger name and origin.

## A1B0 — unused capacity already assigned to one descriptor

```c
uint32_t SYS_A1B0(uint16_t file_id);
```

Resolution failure returns `0`. Success returns:

```text
descriptor->max_size - descriptor->current_size
```

using `+0x0C` and `+0x04`. This is unused capacity inside the descriptor's
current maximum allocation, not total system free space.

AlphaWord Plus contains thirteen A1B0 callers and AlphaQuiz contains two.
Callers promote a 16-bit token into the ABI slot. AlphaWord Plus also uses
reserved token `0x00CB`, proving the operation applies to the clipboard
 descriptor as well.

## A1BC — descriptor maximum capacity

```c
uint32_t SYS_A1BC(uint16_t file_id);
```

Resolution failure returns `0`; success returns descriptor `max_size` from
`+0x0C`.

No direct caller was found in the scanned AlphaWord Plus, AlphaQuiz, KeyWords,
Calculator or ControlPanel applets. The handler-level ABI is nevertheless
explicit and equivalent in all three ROMs.

## Exact relation with A1B4

For an ordinary resolvable token A1B4 returns `current_size`, therefore:

```text
SYS_A1B4(file_id) + SYS_A1B0(file_id) == SYS_A1BC(file_id)
```

or:

```text
current_size + (max_size-current_size) == max_size
```

Do not apply this identity to A1B4 special selectors `0xFC..0xFF`.

## A1AC — global/reclaimable storage accounting

```c
uint32_t SYS_A1AC(void);
```

A1AC takes no public arguments and returns a 32-bit character/storage count. It
is not a naive sum of `max_size-current_size`. Allocation accounting uses
512-byte (`0x200`) units and mask `0xFFFFFE00` to round reclaimable regions down
to a 512-byte boundary.

### Clipboard contribution

For the dedicated clipboard descriptor:

```text
reclaimable = (max_size - previous_or_recoverable_size) & ~0x1FF
if reclaimable <= 0x200:
    reclaimable = 0
```

The accumulated result begins with this contribution.

### Unallocated tail

The routine adds the raw gap between the end of the last allocated descriptor
storage region and the global end-of-file-storage pointer.

### Descriptor reclaimable contribution

For each relevant descriptor A1AC chooses the larger of:

```text
previous_or_recoverable_size (+0x08)
min_size                     (+0x10)
```

It subtracts this watermark from `max_size`, rounds down to a 512-byte boundary,
and counts the region only when the rounded amount is greater than one 512-byte
block.

With an active descriptor, the contribution is applied to that descriptor.
With no active descriptor, the routine walks the full `0x48`-byte descriptor
table, excluding the separately accounted clipboard descriptor.

The result is therefore a **global allocator/reclaimable-space metric**.

## Relation to A1B4 and A1B8

The same `min_size` field appears in neighboring accounting/state operations:

- A1B4 selector `0xFE`: sum `min(current_size, min_size)` over the current group;
- A1B4 selector `0xFF`: same sum over the full descriptor table;
- A1B8 command `-5`: set `min_size = max(current_size, 0x200)` while retaining
  current logical size.

This cross-use is now explained by A1E8's explicit creation-time `min_size`
argument rather than by an unnamed threshold hypothesis.

## AlphaWord Plus caller correlation

AlphaWord Plus contains two A1AC callers and thirteen A1B0 callers. The A1AC
calls occur in routines building character/page storage-status displays; the
same binary has visible labels `Characters used`, `Characters available`,
`Pages used`, `Pages available`, and separate file/System columns.

This supports A1AC as a system-level availability/accounting metric while A1B0
participates in per-file capacity calculations. It is supporting evidence, not
a recovered function name.

## Historical-source result

The original 2000 `FileModule.c/.h` has no public function directly corresponding
to A1AC/A1B0/A1BC. The early implementation used fixed per-file capacities,
while later System 3 introduced dynamic allocation/accounting. Names such as
`FileGetFreeSpace` remain descriptive inventions and are not promoted.

## Regression requirements

Minimum emulator checks:

1. ordinary token: A1B4 + A1B0 == A1BC;
2. unresolved token: A1B0/A1BC return zero;
3. clipboard token `0x00CB`: A1B0 returns clipboard `max-current`;
4. A1AC changes consistently as reclaimable slack crosses 512-byte boundaries;
5. a rounded reclaimable region <= `0x200` is excluded;
6. clipboard is not double-counted in the general descriptor loop;
7. changing `min_size` through a controlled A1B8 `-5` case changes accounting
   consistently with the formulas above;
8. A1E8-created descriptor stores the requested `min_size` at `+0x10` and uses
   `max(requested_max,min_size,0x200)` allocation.

No emulator or hardware regression is claimed as already executed.

## Closure status

- A1B0 mechanics: closed, confidence A; public name pending.
- A1BC mechanics: closed, confidence A; public name pending.
- A1AC mechanics: closed, confidence A; public name pending.
- Descriptor `+0x10 = min_size`: closed, confidence A from A1E8 + ROM diagnostic.
