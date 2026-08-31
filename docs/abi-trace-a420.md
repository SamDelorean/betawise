# ABI trace — A420 / index 264

Public traceability summary for the forward OS3K ABI sweep.

- Disposition: `MECÁNICA_RECONSTRUIDA A / BLOQUEADO` only in contractual return intent.
- AS3000 2005 and NEO 2005 contain non-pointer/unresolved table values; only NEO 2013 is callable.
- NEO 2013 handler: runtime `0x00433DAE`, file `+0x023DAE`, length `0x32`, SHA-256 `b6eb5df5a0e11fad59e68e74dacac6d2bd1655faaf720dc5df82b67e59626392`.
- This is the independently delimited `0x32`-byte routine immediately following A3DC that earlier A3DC analysis explicitly excluded from its handler.
- External physical ABI: three 32-bit slots. Effective use is full32 / low8 / pointer-to-four-longwords.
- A420 expands those inputs into A3DC's seven-slot ABI as `[arg1, sign_extend8(arg2), p[0], p[1], p[2], p[3], 0]`.
- A420 cleans the seven internal slots and propagates A3DC `D0` without local redefinition.
- Direct NEO 2013 firmware callers: none found; official SmartApplet slot: structurally absent.
- No return consumer fixes width or signedness, so no callable prototype is asserted.
- Static regression: `OVERALL PASS`, 23/23 assertions. Dynamic regression: not executed.

Private ROM extraction, instruction listing and regression source/output remain outside the repository.
