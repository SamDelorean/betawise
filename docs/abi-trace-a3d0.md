# ABI trace — A3D0 / index 244

Forward-sweep disposition: **MECÁNICA_RECONSTRUIDA / BLOQUEADO — RETORNO_CONTRACTUAL=DESCONOCIDO**.

- AS3000 2005: non-pointer/unresolved slot; no callable contract.
- NEO 2005: non-pointer/unresolved slot; no callable contract.
- NEO 2013: real handler; two physical argument slots are demonstrated (full pointer + low-16-bit byte count).
- Exactly one direct NEO 2013 firmware BSR caller is demonstrated. It removes 8 bytes of arguments and does not consume `D0`.
- Official SmartApplets: no physical A3D0 table slot in the validated 41-binary corpus.
- The handler defines complete `D0` mechanically, but no independent evidence fixes the intended C return type, width, or signedness.
- No vendor symbol, error enum, or semantic argument names are claimed.
- Static regression: PASS. Dynamic regression: not executed.

Detailed ROM mappings, hashes, CFG/disassembly, helper/global analysis and caller workpapers are retained privately in Drive under the A3D0 dossier.
