# A358 / `memchr` closure

A358 is mechanically closed against the three canonical OS3K firmware generations.
The existing public declaration is correct:

```c
void *memchr(const void *ptr, int value, size_t num);
```

## Mechanical evidence

The A358 handler is exactly 0x26 bytes and byte-identical in AS3000 2005,
NEO 2005 and NEO 2013. Its SHA-256 is
`2184ed0c68ea78432497e5b5449a1e080c3099b70e7582a13b9d18fa0c9f235a`.
It has three physical 32-bit argument slots: a full pointer, the low byte of the
second slot as the comparison value, and a full 32-bit count.

The handler computes `end = ptr + num`, scans bytes in `[ptr,end)`, and returns
the exact address of the first byte equal to the comparison byte. If no byte
matches, or if `num == 0`, it returns NULL. The return is a full pointer/NULL in
D0.L. This dataflow independently establishes `memchr`; the historical index-214
mapping is only secondary corroboration.

## Callers and regression

The corrected PC-index detector was run across the full official SmartApplet
corpus. Four executable A358 callers were found, all in NEO applets:
AcceleratedReader has two sites, Keywords Wireless one, and MathFacts in a Flash
one. Each constructs exactly three longword slots and cleans 0x0C bytes. Observed
counts include 0x400 and 0x800; the Keywords Wireless site supplies a dynamically
computed full-long count. Callers copy or test D0.L as a pointer/NULL result.
The remaining 38 official applets are negative.

A private reproducible static regression rechecks canonical ROM hashes, vector
addresses, exact handler bytes, three-slot accesses, bounded-scan control flow,
match-pointer correction, NULL path, and cross-ROM identity. It executed with
`OVERALL PASS` for all three ROMs. Dynamic/emulator validation has not been
executed and is not claimed here.

No behavior beyond the mechanically demonstrated bounded byte search is inferred.
Firmware bytes and extensive disassembly remain private in the project workpapers.
