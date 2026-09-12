# AlphaSmart 3000 System 3 file storage — physical layer

This note correlates the reconstructed System 3 File API with the production
AlphaSmart 3000 memory map and the official user documentation. Its purpose is
to separate three questions that are easy to conflate:

1. what the File API exposes to software;
2. which physical memory can implement that contract on the AS3000; and
3. how file contents survive normal power-off.

No ROM image, proprietary disassembly, or firmware byte sequence is included
here.

## Production AS3000 memory relevant to files

The production hardware/source documentation gives the normal post-startup
address map as:

```text
0x00000000..0x0003ffff  SRAM, 256 KiB
0x00400000..0x004fffff  Flash, 1 MiB
0x00600000..0x00607fff  write-latch window
0xfffff000..0xffffffff  MC68EZ328 internal registers
```

The SRAM device identified on production hardware is a HY62U8200-class
256 KiB x8 CMOS SRAM. The production Flash is a 28F008B3-class 1 MiB x8
part. Startup chip-select initialization independently maps the SRAM at address
zero and Flash at `0x00400000`.

This matters because the System 3 File API treats file payload storage as
ordinary byte-addressable writable memory.

## File API evidence

The reconstructed descriptor has, among other fields:

```text
+0x00  storage pointer
+0x04  current_size
+0x08  previous/recoverable_size
+0x0c  max_size
+0x10  min_size
+0x14  cursor
```

`FileGetFileInfo` returns the descriptor's `storage` pointer and can register
live mirrors for storage, current size, maximum size, and cursor.

`FileWriteBuffer` writes directly into this storage:

- append copies to `storage + current_size`;
- insert moves the existing suffix in place, then copies at `storage + cursor`;
- overwrite copies to `storage + cursor` without moving the suffix.

These paths use normal memory copy/move semantics. They do not contain a Flash
program/erase command sequence or a separate persistence stage.

`FileSmashFile` likewise zero-fills the active descriptor's entire `max_size`
region and clears its logical state.

`FileClose` is particularly useful as a negative result: mechanically it only
clears the global active-descriptor binding. It does not copy file contents,
program Flash, flush a cache, or invoke a storage-commit helper.

Taken together, the File API is an in-place mutable-memory design rather than a
"load from persistent store -> edit temporary buffer -> save on close" design.

## Why the AS3000 payload storage is SRAM

On the production AS3000, the File API's ordinary copy, move, overwrite, and
zero-fill operations require directly writable byte-addressable memory. The
normal external memory map provides exactly one bulk region with those
semantics: the 256 KiB SRAM at `0x00000000..0x0003ffff`.

The 28F008B3 Flash cannot be treated as equivalent ordinary RAM: programming
and erasing it requires its device command protocol. The File API mutation
handlers described above do not perform that protocol.

Therefore the physical interpretation for the AS3000 System 3 file payload is:

```text
File API descriptor
        |
        +--> storage pointer
                 |
                 +--> allocation inside AS3000 SRAM
```

The exact start and end of the File API allocator pool *within* the 256 KiB SRAM
are still to be reconstructed. This note does not claim that all SRAM belongs
to the file allocator; low RAM also contains vectors, System state, descriptor
tables, applet state, and other runtime structures.

## How contents survive power-off

The original AlphaSmart 3000 User's Guide documents a secondary 3 V lithium
coin cell (CR2032) whose purpose is to power the memory and prevent data loss
when the unit is off and the primary batteries are dead or removed.

That hardware behavior supplies the persistence mechanism required by the File
API model above. Normal file persistence therefore does not require a shutdown
copy to Flash: the mutable file payload remains in battery-backed SRAM.

This also explains why loss or removal of the backup battery can destroy user
files even though the operating system and SmartApplet code in Flash remain
intact.

## System 3 dynamic allocation matches the public UI

The 2005 *AlphaSmart 3000 System 3 Addendum* exposes the same concepts that the
reconstructed allocator uses internally. AlphaSmart Manager can configure a
minimum and a maximum AlphaWord Plus file size. The manual gives:

- minimum file size: 512 characters;
- default maximum: 51,200 characters;
- maximum configurable size: 102,400 characters.

The documented minimum of 512 is exactly `0x200`, matching the allocator floor
used by dynamic descriptor creation:

```text
effective_max = max(requested_max_size, min_size, 0x200)
```

The same manual explains that the total device space remains shared: making
some files larger can reduce the space available to other files and data. This
is consistent with the reconstructed global/reclaimable allocator accounting,
not with eight permanently fixed file buffers.

The historical 1998 AS3000 requirements document is useful here as a chronology
check: it proposed equal fixed-size files. System 3 in 2005 had evolved beyond
that early design into the dynamic minimum/maximum allocation model recovered
from firmware.

## Generational corroboration from NEO work

Independent NEO emulator/reverse-engineering work has also observed low-RAM
AlphaWord descriptor records containing payload pointers and direct mutable
buffers. This is useful corroboration for the System 3 design lineage, but NEO
addresses must not be projected onto the AS3000. The AS3000 physical conclusion
above is based on its own memory map plus the three-generation File API
mechanics.

## Confidence and remaining boundary

**Confirmed / confidence A**

- production AS3000 has 256 KiB SRAM mapped at `0x00000000..0x0003ffff`;
- File API descriptors contain a direct storage pointer;
- File API writes, inserts, overwrites, moves and zero-fills payload in place;
- `FileClose` does not flush data to another medium;
- the AS3000 backup CR2032 powers memory for retention;
- System 3 exposes dynamic minimum/maximum file sizing and a 512-character
  minimum, matching the allocator's `0x200` floor.

**Strong physical conclusion**

- ordinary System 3 file payload storage on the production AS3000 is allocated
  in the battery-backed SRAM rather than being saved to Flash on close.

**Still unresolved**

- exact base/end globals of the AS3000 File API SRAM pool;
- allocator placement/compaction algorithm at the physical-pointer level;
- exact SRAM ranges reserved for System state versus descriptors versus file
  payloads;
- startup integrity/recovery metadata and the complete cold/warm recovery path;
- whether any exceptional maintenance/update path serializes selected file data
  elsewhere. Such a path is not part of ordinary `FileWriteBuffer`/`FileClose`
  operation.

## Related reconstructed interfaces

See also:

- `docs/file-api-reconstruction.md`
- `docs/file-space-accounting.md`
- `docs/file-identity-dynamic-closure.md`
