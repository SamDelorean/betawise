# A380 / `strcat` closure

A380 (index 224) is mechanically reconstructed as:

```c
char *strcat(char *dst, const char *src);
```

## Primary mechanics

The canonical AS3000 2005, NEO 2005 and NEO 2013 ROMs resolve A380 to the following entry points:

- AS3000 2005: runtime `0x004DDBCE`, file `+0x01DBCE`
- NEO 2005: runtime `0x005E02EC`, file `+0x0202EC`
- NEO 2013: runtime `0x004366EA`, file `+0x0266EA`

The contractual handler is 0x1C bytes and byte-identical in all three ROMs (SHA-256 `59190d0e290ade7eb5b8ab112af48a58ba4fb1e1a8a7b8a98e344ec1d6ef2f83`). It consumes two pointer-sized slots. It scans `dst` through its terminating NUL, backs up to that terminator, copies `src` byte by byte including its terminating NUL, and deliberately returns the original destination pointer in `D0.L`.

There is no capacity argument, NULL validation, global state, helper dependency or bounds check in this entry point.

## Caller correlation

The validated official SmartApplet corpus is complete at 41/41. The corrected A-line table detector finds 130 executable A380 calls in 16 table-bearing applets, 14 table-bearing negatives and 11 structural negatives. Representative callers push the source pointer first and the destination pointer second, then remove exactly 8 bytes after the trap, matching the two-argument C calling convention.

No one of the 130 SmartApplet callsites needs to consume the returned `D0` before a later call or register overwrite. That does not make the return residual: the handler explicitly preserves the original destination solely to move it into `D0.L` immediately before `RTS`.

Direct absolute firmware JSR counts are 9/9/9 for AS3000 2005 / NEO 2005 / NEO 2013; no direct JMP entry use was found.

## Adversarial alternatives

- `strcpy` is rejected because A380 first scans the existing destination to its NUL.
- `strncat` is rejected because there is no third count argument or bounded-copy logic.
- `memcpy` is rejected because there is no count argument and copying terminates on the source NUL.
- A void proprietary append is rejected because the original destination is deliberately preserved and reconstructed as the terminal `D0.L` result.

The historical BetaWise index-224 `strcat` mapping is therefore corroborative rather than the basis of the identification.

## Validation status

Static regression over the canonical ROMs and official SmartApplet manifest executed with **OVERALL PASS**. It revalidates the handler hash, A380 caller total (130), positive applet count (16), and detector controls A36C=99 and A378=598. Dynamic emulator-first regression was not executed for this mechanically trivial entry.

Status: **MECÁNICA_CERRADA A / PUBLICADO**. The existing `os3k.h` declaration already has the correct two-pointer `char *` contract, so no header modification is required.
