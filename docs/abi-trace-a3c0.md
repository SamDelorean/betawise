# Forward-sweep trace — A3C0 / index 240

- Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.
- Source correlation: no independently reproducible vendor symbol/prototype recovered; `SYS_A3C0` remains neutral.
- Platform availability: AS3000 2005 and NEO 2005 contain non-pointer marker `0x89909192`; no callable contract is asserted. NEO 2013 contains the real handler.
- NEO 2013 raw ABI: `uint8_t SYS_A3C0(void)`; zero physical arguments.
- Handler: runtime `0x0043DAD2`, file `0x02DAD2`, length `0x10`, SHA-256 `f3ebf233e2aba5b86449eac3049f4376acbf9902ba628177308aee75d1085bc7`.
- Mechanism: tests one mutable backing byte and normalizes zero/nonzero to `D0.B = 0/1`.
- Firmware caller evidence: fresh ROM-wide scan reproduces four direct JSRs; one consumes `CMPI.B #1,D0`, three consume `TST.B D0`. Upper `D0` is therefore non-contractual.
- Official corpus: existing 41/41 regression remains PASS; 16 extended applets contain the slot but have zero executable A3C0 calls, 14 are legacy and 11 structural negatives.
- Backing-state semantics remain unknown; references to values beyond the normalized output domain prevent typing the storage itself as boolean.
- Fresh source-first directed regression: **EJECUTADA / PASS**. Dynamic regression: **ESPECIFICADA / NO EJECUTADA**.

Public functional evidence: [`sys-a3c0-neo2013-closure.md`](sys-a3c0-neo2013-closure.md).

Firmware bytes, detailed callers, corpus hashes, and private reverse-engineering workpapers remain in Drive.
