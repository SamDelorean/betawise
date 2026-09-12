# System 3 / OS3K file allocator — three-generation physical closure

Status: **mechanics closed A / source-first / three-generation correlation**.

This note closes the physical allocator model behind the reconstructed System 3
File API. It correlates the November 2005 AlphaSmart 3000 image, November 2005
NEO image, and July 2013 NEO/System 3 image without publishing ROM bytes or
proprietary disassembly.

The public ABI remains documented in `file-api-current-reference.md`. This note
covers the internal SRAM geometry and allocation movement needed by an emulator.

## 1. Canonical images used

The analysis was revalidated against the three canonical project images already
used by the ABI reconstruction corpus:

| generation | SHA-256 |
| --- | --- |
| AlphaSmart 3000 System 3, Nov 2005 | `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708` |
| NEO System 3, Nov 2005 | `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d` |
| NEO/System 3, Jul 2013 | `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0` |

## 2. Three-generation SRAM geometry

| structure | AS3000 2005 | NEO 2005 | NEO 2013 |
| --- | ---: | ---: | ---: |
| file-group table | `0x0F42` | `0x0F9A` | `0x0FD2` |
| descriptor table | `0x1042` | `0x109A` | `0x10D2` |
| active-descriptor global | `0x3442` | `0x349A` | `0x34D2` |
| last-descriptor global | `0x3446` | `0x349E` | `0x34D6` |
| clipboard-descriptor global | `0x344A` | `0x34A2` | `0x34DA` |
| file-storage-end global | `0x354E` | `0x35A6` | `0x35DE` |
| RAM-valid signature address | `0x3566` | `0x35BE` | `0x35F6` |
| RAM-valid signature value | `0x0D89` | `0x0D89` | `0x0D89` |
| file arena base | `0x4558` | `0x69C0` | `0x1509C` |
| file arena end, exclusive | `0x3D800` | `0x7D800` | `0x7D800` |
| descriptor stride | `0x48` | `0x48` | `0x48` |
| descriptor slots | 128 | 128 | 128 |

The physical arena sizes are therefore:

| generation | arena bytes | bytes after initial 0x200-byte clipboard allocation |
| --- | ---: | ---: |
| AS3000 2005 | 234,152 | 233,640 |
| NEO 2005 | 486,976 | 486,464 |
| NEO 2013 | 427,876 | 427,364 |

These are physical byte ranges, not a promise that every byte is independently
allocatable through one public operation. Normal allocator redistribution uses
`0x200`-byte accounting/alignment rules, while a few internal boundary cases can
assign a raw end-of-arena span.

The NEO 2013 arena starts substantially higher than NEO 2005 while both share
`0x7D800` as the file-storage ceiling. The later System therefore reserves more
low RAM for runtime/System state and leaves a smaller file arena, without
changing the descriptor/allocator architecture.

## 3. Descriptor ordering is also physical allocation ordering

The descriptor table is not merely a namespace table. In normal state its
storage pointers and `max_size` fields describe adjacent portions of the file
arena.

The allocator preserves the practical invariant:

```text
next.storage == current.storage + current.max_size
```

while capacity is transferred between neighboring descriptors. Descriptor
records can be inserted/removed independently of payload bytes, but the payload
allocator repairs the corresponding storage pointers and capacities so that the
ordered partition remains coherent.

This explains why descriptor `+0x00` is a direct SRAM pointer and why
`FileGetFileInfo` can expose it to an applet.

## 4. Guarded SRAM move helper

All three generations contain the same internal overlap-safe byte mover used by
the file allocator. Its core move routine is byte-identical across the three
canonical images and selects forward or backward copying according to overlap,
i.e. `memmove` semantics rather than `memcpy` semantics.

The file-specific wrapper validates the entire source and destination ranges
before calling that mover:

- lower allowed address: `0x00000400` in all three generations;
- upper allowed address: `0x00040000` on AS3000;
- upper allowed address: `0x00080000` on NEO 2005/2013.

Out-of-range moves take an internal fatal diagnostic path. The ordinary file
allocator therefore treats payload storage as directly writable SRAM and moves
file contents inside the physical RAM address space; no Flash program/erase
operation participates in these paths.

## 5. Internal allocator redistribution helper

The central allocator helper has no recovered public symbol, so it remains
unnamed here. Its mechanics are stable across generations.

The NEO 2005 and NEO 2013 implementations are 794 bytes each, with 785/794 bytes
identical at the same positions; differences are relocated globals/calls and one
hardware-dependent operand. The AS3000 implementation has the same control-flow
model but lacks a NEO-specific hardware-control prologue.

The helper uses:

```text
watermark = max(recoverable_size, min_size)
normal_floor = max(round_up(watermark, 0x200), 0x200)
```

It then transfers capacity between adjacent descriptors. When a descriptor is
shrunk, only the payload range required by its preserved watermark is moved; the
released capacity is absorbed by the neighboring descriptor. After changing a
descriptor it invokes the same live-mirror synchronization helper used by
`FileGetFileInfo` and the public read/write family.

### Moving capacity toward a target

If the requested target is different from the current active descriptor, the
helper computes how much reclaimable aligned capacity the active descriptor can
give up. It then propagates that capacity through the descriptor chain toward
the target. Each step changes two adjacent `max_size`/`storage` values and moves
the preserved bytes with overlap-safe semantics.

This makes the active descriptor the normal elastic reservoir for File API
allocation.

### Target is active, or no active descriptor exists

When the target is the active descriptor, descriptors on either side are reduced
toward their preserved floors and the released capacity is propagated toward the
target. The target consequently absorbs the available slack.

With no target/active descriptor, the same mechanism compacts descriptors toward
their preserved floors and leaves the recovered capacity in the global tail.

This behavior explains the previously closed A1AC rule: with an active file,
reclaimable slack is concentrated around that active allocation; with no active
file, global descriptor slack can be reclaimed into the free tail.

## 6. A1E8 creation now has a physical algorithm

The public reconstruction already established:

```c
int32_t SYS_A1E8(uint8_t token_group, const char *name,
                 uint32_t min_size, uint32_t requested_max_size);
```

and:

```text
effective_max = max(requested_max_size, min_size, 0x200)
```

The physical path is now closed more precisely:

1. insert a new `0x48` descriptor into the ordered descriptor table;
2. repair file-group descriptor pointers and canonical tokens;
3. seed the new storage pointer from the arena base, previous allocation end, or
   following descriptor storage according to insertion position;
4. initialize logical size/recoverable size/cursor to zero and install metadata;
5. ask the redistribution helper to obtain at least `effective_max` capacity for
   the new descriptor;
6. if the resulting `max_size` is still too small, run the global reclaim path
   and retry;
7. if the retry is still insufficient, remove the provisional descriptor and
   return raw `-12`.

### Global reclaim retry

The retry path is not an opaque allocator call. It:

- detaches the active file context;
- logically clears clipboard current/recoverable size and cursor;
- snapshots each ordinary descriptor's current size into its recoverable/high-
  water field;
- invokes allocator redistribution with no target so allocations shrink toward
  their current/minimum requirements;
- retries allocation for the new descriptor.

Thus A1E8 first tries to satisfy a new allocation by redistributing ordinary
slack and only then performs a broader reclaim pass.

## 7. A1EC deletion now has a physical algorithm

For an ordinary non-first descriptor, deletion first adds the deleted
`max_size` capacity to the preceding descriptor. Its payload does not need to be
moved at that step.

For the first descriptor, where no predecessor exists, the preserved portion of
the following descriptor is moved down to the deleted descriptor's storage
address. The following descriptor then inherits that storage pointer and absorbs
the deleted capacity.

The descriptor table is subsequently compacted in `0x48`-byte records, group
pointers are repaired, canonical tokens are renumbered, and the last-descriptor
pointer moves back by one record.

Finally the central redistribution helper is called using the current active
descriptor. If one exists it becomes the elastic allocation that absorbs the
newly available capacity; with no active descriptor, allocations are compacted
toward their preserved floors instead.

This is stronger than the earlier generic statement that A1EC "reclaims an
allocation": the physical ownership transfer and payload movement are now
reconstructed.

## 8. File-system validator is separate from allocation

A helper called after A1E8/A1EC with a source-line value had previously been
safe to describe only as allocator maintenance. Its role is now separable: it is
an internal file-system validator.

It checks descriptor data-pointer bounds, `max_size`, current size, `min_size`
and cursor invariants, counts validation errors, and can enter an internal dump
path. It also normalizes the recoverable/high-water field so it is not below
current size or above maximum size.

It does **not** perform the payload allocation redistribution described above.
That distinction should be preserved in future documentation and emulator code.

## 9. Persistence model

The same RAM-valid signature value `0x0D89` is used in all three generations,
although its address is relocated. Initializer/recovery paths combine that
signature with the generation-specific group table, descriptor table and file
arena addresses.

This strengthens the existing physical conclusion for AS3000 and establishes the
same design lineage on NEO: normal file state is a live SRAM-resident structure,
not a temporary edit buffer waiting for `FileClose` to serialize it to Flash.

## 10. Emulator consequences

A faithful emulator should model the file subsystem as a persistent SRAM arena,
not as eight independent host files hidden behind the ABI. In particular it
needs to preserve:

- ordered `0x48` descriptor records;
- direct storage pointers;
- `current_size`, recoverable/high-water, `max_size`, `min_size` and cursor;
- adjacent-capacity transfer with overlap-safe movement;
- 0x200 accounting/alignment rules;
- active-descriptor elasticity;
- persistent `FileGetFileInfo` live mirrors;
- the generation-specific arena bounds and warm-RAM validity state.

A host-filesystem backend can still be used for persistence, but it should be an
implementation layer beneath this SRAM/allocator model rather than replacing its
observable semantics.

## 11. Regression

Static regression was executed against all three canonical images: **86/86
PASS**.

It verifies canonical hashes, generation-specific geometry, end-boundary
initialization, RAM-valid signature anchors, descriptor/group/arena initializer
anchors, guarded move bounds, the byte-identical three-generation overlap-safe
move core, allocator constants and stride, allocator-to-move/live-sync call
edges, A1E8/A1EC allocator edges, live-mirror offsets, and the NEO05/NEO13
allocator equivalence.

Dynamic emulator regression remains pending. The next useful layer is to test
allocation growth, insertion, deletion and global reclaim on disposable emulator
SRAM snapshots and compare the resulting descriptor/storage topology against
these invariants.
