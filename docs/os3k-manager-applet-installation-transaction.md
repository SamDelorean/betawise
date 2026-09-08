# OS3K Manager applet-installation transaction

This note records a source-first reconstruction of the Manager-side transaction used to install a SmartApplet. It intentionally stops short of assigning firmware-handler semantics that have not yet been verified against the canonical images.

## Source correlation

Three independent public code bases support the same multi-request architecture. Selector `0x06` is only the initialization stage of an applet installation transaction; it is not the request that carries the applet payload itself.

AlphaSync defines `WRITE_APPLET` as request `0x06`, describes request `0x07` as an unknown operation used while writing an applet, and describes request `0x0b` as another unknown operation used while writing an applet. Its response constants associate `0x46` with `WRITE_APPLET`, `0x47` with request `0x0b`, and `0x48` with request `0x07`.

Neotools resolves the host-side ordering explicitly. Its applet installer performs these stages:

1. Send `REQUEST_WRITE_APPLET (0x06)` with allocation/size requirements and wait for `RESPONSE_WRITE_APPLET (0x46)`.
2. Split the applet image into blocks of at most `0x400` bytes.
3. For every block, send `REQUEST_BLOCK_WRITE (0x02)` with block length and checksum, wait for `RESPONSE_BLOCK_WRITE (0x42)`, transfer the raw block bytes, and wait for `RESPONSE_BLOCK_WRITE_DONE (0x43)`.
4. Send `REQUEST_PROGRAMMING_APPLET_BLOCK (0x0b)` and wait for `RESPONSE_PROGRAMMING_APPLET_BLOCK (0x47)` before advancing to the next block.
5. After the last block, send `REQUEST_FINALIZE_WRITING_APPLET (0x07)` and wait for `RESPONSE_FINALIZE_WRITING_APPLET (0x48)`.

The independent `neo-re` protocol implementation corroborates the control selectors directly: its applet-begin helper is tested against `command(0x06, ...)`, its program-applet helper against `command(0x0b, 0, 0)`, and its finalize helper against `command(0x07, 0, 0)`.

Therefore the source-supported transaction is:

`0x06 init -> [0x02 raw-block 0x0b program] * N -> 0x07 finalize`

with each raw block no larger than `0x400` bytes in the neotools implementation.

## Important correction

`WRITE_APPLET (0x06)` must **not** be described as the chunk-transfer request. The chunked payload transport belongs to `BLOCK_WRITE (0x02)` plus the raw USB data transfer; request `0x0b` advances/programs each staged applet block, and `0x07` performs finalization.

The two size fields sent by neotools in request `0x06` are derived from the applet header's ROM requirement and combined RAM/file-space requirement. Their exact firmware-side interpretation, allocation policy, bit packing, error paths, and persistence state remain to be established by canonical-ROM verification.

## Verification boundary

Status of this note: **SOURCE-FIRST CONFIRMED / firmware correlation pending**.

Confirmed here from independent public host implementations:

- `0x06` is initialization for applet installation, response `0x46`;
- applet bytes are transferred in blocks of at most `0x400` by the observed neotools host implementation;
- `0x02` announces each block and its checksum, with responses `0x42` and `0x43` around the raw block transfer;
- `0x0b` is issued after every block and receives `0x47` in the neotools transaction;
- `0x07` finalizes the installation and receives `0x48`;
- `neo-re` independently confirms the control-role mapping `0x06` begin, `0x0b` program block, and `0x07` finalize.

Not yet promoted to a firmware contract:

- exact branch bounds for cases `0x06`, `0x07`, `0x0b`, and their interaction with case `0x02`;
- register-level argument interpretation;
- allocation and rollback semantics;
- exact validation performed during finalization;
- internal helper/vendor names.

The next canonical-ROM pass should treat the four selectors as one transaction family rather than trying to infer case `0x06` in isolation. That pass should verify the shared transfer-state globals and the transitions `init -> block staging -> program -> finalize`, then compare those mechanics across AS3000, NEO 2005, and NEO 2013.

No A-line ABI entry is implied. The Manager request namespace is independent of the A-line syscall frontier.
