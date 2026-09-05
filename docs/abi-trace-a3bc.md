# Forward-sweep trace — A3BC / index 239

- Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.
- Source correlation: no independently reproducible vendor symbol or semantic name recovered; `SYS_A3BC` remains neutral.
- Platform availability: AS3000 2005 and NEO 2005 contain non-pointer marker `0x83848788`; no callable contract is asserted. NEO 2013 contains the real handler.
- NEO 2013 raw ABI: `uint8_t SYS_A3BC(void)`; zero physical arguments.
- Handler: runtime `0x0043DACA`, file `0x02DACA`, length `0x08`, SHA-256 `cca0b14256241ab7c00bc1a88c952d1d044674466f3a74afd5823b34ffdea792`.
- Private getter: length `0x08`, SHA-256 `dda45819112d6ce88c5b5984fa875ebd64a09a67b00839deaa4fa75b63a26bfe`; reads one mutable global byte into `D0.B`.
- Firmware caller evidence: fresh ROM-wide scan reproduces exactly one direct JSR; the next operation is `TST.B D0`, independently proving byte-sized return consumption.
- Backing storage: observed `MOVE.B #1` and `CLR.B` writers confirm byte-sized mutable state but do not justify a boolean/status/vendor name.
- Official caller corpus: existing 41/41 static regression remains PASS; 0 executable SmartApplet A3BC callers.
- Fresh source-first directed regression: **EJECUTADA / PASS**. Dynamic regression: **ESPECIFICADA / NO EJECUTADA**.
- Vendor symbol and semantic meaning of the backing byte remain **DESCONOCIDOS**.

Public functional evidence: [`sys-a3bc-neo2013-closure.md`](sys-a3bc-neo2013-closure.md).

ROM bytes, complete private disassembly, global-reference analysis, caller details, applet hashes, and regression workpapers remain private in Drive.
