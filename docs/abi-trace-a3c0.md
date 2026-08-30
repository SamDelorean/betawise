# Forward-sweep trace — A3C0 / index 240

- Status: mechanical A, published, complete.
- Platform availability: AS3000 2005 and NEO 2005 contain non-pointer marker `0x89909192`; no callable contract is asserted. NEO 2013 contains the real handler.
- NEO 2013 neutral ABI: `uint8_t SYS_A3C0(void)`.
- Handler: runtime `0x0043DAD2`, file `0x02DAD2`, length `0x10`, SHA-256 `f3ebf233e2aba5b86449eac3049f4376acbf9902ba628177308aee75d1085bc7`.
- Mechanism: tests one mutable global byte and normalizes zero/non-zero to `D0.B` = 0/1; no input arguments.
- Firmware caller evidence: four direct JSRs; one immediately executes `CMPI.B #1,D0` and three execute `TST.B D0`. Upper `D0` is therefore non-contractual.
- Official corpus: 41/41. Sixteen extended applets physically contain the A3C0 slot but have zero executable PC-index calls; 14 legacy tables end before it; 11 are structural negatives. Caller count = 0/41.
- Backing-state semantics and vendor symbol remain unknown; neighboring writes prevent reducing the global itself to a boolean type.
- Static regression: OVERALL PASS. Dynamic regression: not executed.

Public functional evidence: [`sys-a3c0-neo2013-closure.md`](sys-a3c0-neo2013-closure.md).

Firmware bytes, detailed callers, and private reverse-engineering workpapers remain in Drive.
