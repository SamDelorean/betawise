# Forward-sweep trace — A3B8 / index 238

- Status: mechanical A, published, complete.
- Platform availability: AS3000 2005 and NEO 2005 contain non-pointer marker `0x05060708`; no callable contract is asserted for those generations. NEO 2013 contains the real handler.
- NEO 2013 neutral ABI: `uint8_t SYS_A3B8(uint16_t arg1, void *arg2, uint16_t arg3, uint8_t arg4)`.
- Handler: runtime `0x0043ECEE`, file `0x02ECEE`, length `0x50`, SHA-256 `495fb94ec6eaef43abcf182c5ba94b95d421afa4a0866cb0806f81a5306ca143`.
- Official caller corpus: 41/41; exactly one executable A-line caller, NEO ControlPanel. Four functional slots are demonstrated; a fifth cleaned stack slot is retained state from an earlier call and is not an A3B8 parameter.
- Return: only `D0.B` is contractual from the demonstrated caller and handler staging.
- Static regression: OVERALL PASS. Dynamic regression: not executed.
- Vendor symbol, argument semantics, status enum, retry reason, and secondary private dependency remain unknown.

Public functional evidence: [`sys-a3b8-neo2013-closure.md`](sys-a3b8-neo2013-closure.md).

ROM bytes, complete disassembly, helper listing, caller reconstruction, and regression workpapers remain private in Drive.
