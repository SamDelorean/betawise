# A390 / `strlen` closure

A390 (index 228) is reconstructed and revalidated source-first as:

```c
size_t strlen(const char *str);
```

Status: **CLOSED A / SOURCE_FIRST / PUBLISHED**.

## Source-first anchors

The historical references were consulted before reopening the firmware:

- the preserved BetaWise `syscall.c` maps index 228 to `strlen`;
- the preserved SDK/API declaration in `os3k.h` / `os3k.pdf` is `size_t strlen(const char *str)`;
- `debug.pdf` contains ordinary call sites such as `len = strlen(pBuffer)` and `char len = strlen((char*)gd->buffer[i])`, corroborating NUL-terminated string-length use while also demonstrating that individual callers may truncate the result after the call;
- the current public BetaWise header retains the same prototype;
- A06C is separately reconstructed as a strlen-equivalent OS service, so its existence was treated as an adversarial comparison rather than evidence that A390 must share its identity.

These references are naming/signature anchors only. The closure below is based on a fresh read of the three canonical firmware images.

## Fresh primary verification

The three canonical ROM files were fetched again from the private project corpus and their complete SHA-256 values were revalidated:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Canonical entry points remain:

- AS3000 2005: runtime `0x004DDE24`, file `+0x01DE24`
- NEO 2005: runtime `0x005E0784`, file `+0x020784`
- NEO 2013: runtime `0x00436B82`, file `+0x026B82`

A fresh byte extraction at each entry produced the same 0x12-byte handler in all three images, with handler SHA-256 `46193c2eda4c7b66d5a1fd34b8e73773c52f87f49d0e60439c29c9daa95798f5`. Independent 68k decoding of that sequence gives the same control flow in every generation: load the sole pointer argument from `4(SP)`, preserve its starting address, test and post-increment one byte at a time until the terminating NUL, subtract the original pointer from the post-NUL cursor, subtract one, and return through `D0.L`.

Therefore, for a valid NUL-terminated input pointer, the handler returns the number of bytes preceding the first NUL. An empty string returns zero. There is no NULL guard: a null or otherwise invalid pointer violates the handler's precondition and is not assigned a defined API result by this reconstruction.

No helper call, global access, table access, write to the input string, bounded-count argument, or generation-specific branch exists in the contractual handler. Materially visible scratch state is confined to `A0`, `A1`, `D0` and condition-code effects of the executed arithmetic/tests; no stronger preservation claim is made beyond the ABI evidence.

## Caller correlation

The previously executed complete official-corpus regression remains reproducible: 477 executable A390 calls in 26 table-bearing applets, four table-bearing negatives and eleven structural negatives. Representative callers remove exactly one 32-bit argument slot and then consume `D0.L` with full-long operations such as `TST.L D0` and `MOVE.L D0,D6`. Direct firmware JSR counts are 36/37/53 for AS3000 2005 / NEO 2005 / NEO 2013, with no direct JMP entry use found.

`debug.pdf` supplies independent source-level use of `strlen` on character buffers. Its examples do not establish the trap identity by themselves, but they are consistent with the firmware-derived contract and help distinguish API return width from narrower caller-local storage.

## Adversarial checks

The dominant `strlen` hypothesis was explicitly challenged:

- character search is incompatible because the handler has no character argument and returns pointer-difference arithmetic rather than a match address;
- `strnlen` is incompatible because no bound/count is consumed;
- a void scanner is incompatible because the handler explicitly computes `D0.L` and callers consume it;
- a 16-bit contractual return is incompatible with the full-long handler arithmetic and full-long caller consumption;
- A06C's separate strlen-equivalent behavior does not collapse the two ABI entries: A390 has an independently preserved standard-library mapping and independently verified handler/callers.

No source/firmware contradiction was found. The historical name and prototype survive primary verification unchanged.

## Regression status

Static regression over canonical ROMs and the official SmartApplet manifest was previously **EXECUTED / OVERALL PASS**. During the original mechanical closure its first run failed closed because the A36C positive-control physical offset had been mis-entered; after correcting the compacted-table mapping, a fresh run passed ROM hashes, exact handler bytes/hash, caller counts, positive controls and full-long return consumers.

For this source-first revalidation the three full canonical ROM hashes and the three A390 handler extractions were executed again directly and matched the recorded canonical evidence exactly. Dynamic/emulator regression remains **SPECIFIED / NOT EXECUTED**; it is not being represented as executed.

The existing `os3k.h` declaration already matches the verified contract, so no header change is required.
