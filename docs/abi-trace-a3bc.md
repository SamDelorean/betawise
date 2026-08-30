# Forward-sweep trace — A3BC / index 239

- Status: mechanical A, published, complete.
- Platform availability: AS3000 2005 and NEO 2005 contain non-pointer marker `0x83848788`; no callable contract is asserted for those generations. NEO 2013 contains the real handler.
- NEO 2013 neutral ABI: `uint8_t SYS_A3BC(void)`.
- Handler: runtime `0x0043DACA`, file `0x02DACA`, length `0x08`, SHA-256 `cca0b14256241ab7c00bc1a88c952d1d044674466f3a74afd5823b34ffdea792`.
- Private getter: length `0x08`, SHA-256 `dda45819112d6ce88c5b5984fa875ebd64a09a67b00839deaa4fa75b63a26bfe`; reads one mutable global byte.
- Firmware caller evidence: exactly one direct JSR; it immediately performs `TST.B D0`, proving byte-sized return consumption. A3BC reads no stack arguments.
- Official caller corpus: 41/41; 0 executable A3BC SmartApplet callers. Sixteen extended-table applets were rematerialized/rehashed and scanned; 14 legacy tables end before A3BC; 11 are structural negatives.
- Static regression: OVERALL PASS. Dynamic regression: not executed.
- Vendor symbol and semantic meaning of the backing byte remain unknown; observed zero/one writers are not promoted to a boolean/status name.

Public functional evidence: [`sys-a3bc-neo2013-closure.md`](sys-a3bc-neo2013-closure.md).

ROM bytes, complete private disassembly, global-reference analysis, caller details, applet hashes, and regression workpapers remain private in Drive.
