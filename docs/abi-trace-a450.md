# SYS_A450 / index 276 — ABI trace

Status: **MECANICA_CERRADA A / SOURCE_FIRST**. Remaining source-level naming and scalar-return questions are evidence limits, not sequencing blockers.

## Availability and primary revalidation

- AlphaSmart 3000 (2005): slot 276 is `0xF0F1F2F3`; no callable contract is claimed.
- NEO (2005): slot 276 is `0xF0F1F2F3`; no callable contract is claimed.
- NEO (2013): slot 276 resolves to runtime `0x0044331C`, file offset `0x03331C`.
- Handler: `0x6C` bytes, SHA-256 `c86f30477adac52894edf4eea020bf7009c58ea8ecb35fd56da8daddedfc5669`.
- The following A454 vector points backward (`0x00434980`), so A450 is delimited by its own CFG/terminal RTS rather than by vector monotonicity.

Recovered BetaWise/ioma8 sources establish only neutral `SYS_A450` at index 276; no independent vendor prototype or semantic name was recovered.

## Mechanical call shape

The NEO 2013 wrapper uses five 32-bit stack slots plus a six-byte aggregate result channel through `A1`:

1. arg1: opaque pointer-like input forwarded to A448;
2. arg2: NUL-terminated exact selector forwarded to A448;
3. arg3: writable byte-buffer destination forwarded to A444;
4. arg4: only low 16 bits are consumed and sign-extended as the A444 bound;
5. arg5: writable pointer-like output forwarded to A444.

Stage 1 calls A448 with descriptor-array capacity 2. Stage 2 is entered only when aggregate state is 0 or 8 and produced count is positive; it then sends descriptor 0 to A444 for bounded single-byte XML/entity-reference decoding. No sixth stack slot is demonstrated.

## Official caller evidence

The validated official-corpus sweep previously established exactly three executable callers, all in Accelerated Reader. Fresh directed revalidation reproduced the indexed-PC call opcodes at file offsets `0xD8B4`, `0xDA86`, and `0xDAB4`, plus selector literals `/q`, `/q1`, and `/q2`. The historical full corpus regression remains `98/98 PASS`; the fresh primary-directed regression is `39/39 PASS`.

The repeated `/q*` literals plus the A448/A444 pipeline support a **strong path-like structural-selector inference**, but names such as `GetElementText` or `GetPath` remain speculative.

## Return discipline

A450 does not normalize D0. Paths stopping after A448 inherit A448 D0; paths executing A444 inherit A444 D0. Recovered callers do not consume the syscall D0. Therefore an independent scalar return contract is **not demonstrated**. The six-byte A1 aggregate is the demonstrated structured result channel.

## Publication boundary

ROM bytes, extended disassembly, caller-resolution machinery, and private regression workpapers remain outside the repository. No callable C header is emitted until a source-level contract can represent the aggregate/result semantics without invention.
