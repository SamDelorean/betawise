# Forward-sweep trace — A3B8 / index 238

- Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.
- Source correlation: no independently reproducible vendor symbol/prototype recovered; `SYS_A3B8` remains a neutral project label.
- Platform availability: AS3000 2005 and NEO 2005 contain non-pointer marker `0x05060708`; no callable contract is asserted for those generations. NEO 2013 contains the real handler.
- NEO 2013 neutral raw ABI: `uint8_t SYS_A3B8(uint16_t arg1, void *arg2, uint16_t arg3, uint8_t arg4)`; physical slots are 32-bit and effective consumption is `low16 / pointer / low16 / low8`.
- Handler: runtime `0x0043ECEE`, file `0x02ECEE`, length `0x50`, SHA-256 `495fb94ec6eaef43abcf182c5ba94b95d421afa4a0866cb0806f81a5306ca143`; it ends exactly at A3B4.
- Behavior: invokes private helper `0x0043EEB6`; nonzero status triggers a private recovery dependency with literal `1` and retry. At most six helper attempts occur.
- Return: only `D0.B` is contractual from both handler staging and demonstrated caller consumption.
- Official caller corpus: prior full regression 41/41 PASS; exactly one executable A-line caller, NEO ControlPanel. Four functional slots are demonstrated; a fifth cleaned stack slot is retained state from an earlier call and is not an A3B8 parameter.
- Fresh source-first directed regression: **EJECUTADA / PASS** against the three canonical ROMs plus current ControlPanel object. Dynamic regression: **ESPECIFICADA / NO EJECUTADA**.
- Vendor symbol, argument semantics, status enum, retry reason, and secondary private dependency remain **DESCONOCIDOS**.

Public functional evidence: [`sys-a3b8-neo2013-closure.md`](sys-a3b8-neo2013-closure.md).

ROM bytes, complete disassembly, helper listing, caller reconstruction, and regression workpapers remain private in Drive.
