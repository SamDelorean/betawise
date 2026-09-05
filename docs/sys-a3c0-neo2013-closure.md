# A3C0 / index 240 — source-first generational closure

Status: **CERRADO A / SOURCE_FIRST / PUBLICADO**.

`SYS_A3C0` remains a neutral project label. Historical/repository correlation has not recovered an independent vendor symbol or semantic name for the backing state.

## Platform availability

Fresh canonical-ROM revalidation confirms:

- AlphaSmart 3000 (2005): index 240 contains `0x89909192`, not a demonstrated code pointer.
- NEO (2005): index 240 contains `0x89909192`, not a demonstrated code pointer.
- NEO (2013): index 240 points to runtime `0x0043DAD2` / file `0x02DAD2`.

No callable contract is asserted for the 2005 ROMs and the marker is not assigned vendor semantics.

## NEO 2013 raw ABI

The complete handler reads no input arguments. It tests one mutable byte and normalizes zero/nonzero to a byte result:

```c
/* NEO 2013 only; vendor name and state meaning unknown. */
uint8_t SYS_A3C0(void);
```

Fresh ROM-wide xref scanning reproduces four direct callers. One immediately executes `CMPI.B #1,D0`; the other three immediately execute `TST.B D0`. That independently establishes `D0.B` as the contractual return width. Upper `D0` bits are not contractual because the false path clears only `D0.B`.

## Handler and backing state

Fresh extraction reproduces the exact NEO 2013 handler at file `0x02DAD2`, length `0x10`:

`TST.B $00011944 ; BNE ... ; CLR.B D0 ; BRA ... ; MOVEQ #1,D0 ; RTS`

SHA-256:

`f3ebf233e2aba5b86449eac3049f4376acbf9902ba628177308aee75d1085bc7`

The backing storage is byte-sized. Fresh reference contexts show byte writers and neighboring writes of values beyond the simple normalized output domain. Consequently the underlying state is deliberately **not** published as a C `bool`, named status, enum, or vendor subsystem state. A3C0 itself only maps zero to 0 and nonzero to 1.

## Callers and regression

The four direct NEO 2013 firmware call sites are all byte consumers. The existing SmartApplet regression remains **EJECUTADA / PASS** across the 41-object corpus: 16 extended-table applets physically contain the A3C0 slot but corrected PC-index analysis finds zero executable A3C0 applet callers; 14 legacy tables end before the extension; 11 are structural negatives.

A fresh source-first directed regression covering canonical slot values, exact handler bytes/hash, the four firmware xrefs and byte consumers, and backing-byte references is also **EJECUTADA / PASS**. Dynamic/emulator regression remains **ESPECIFICADA / NO EJECUTADA**.

## Confidence classification

- **CONFIRMADO:** callable implementation only in NEO 2013 among the canonical ROMs.
- **CONFIRMADO:** zero physical arguments.
- **CONFIRMADO:** normalized zero/nonzero return in `D0.B`.
- **CONFIRMADO:** backing state is a mutable byte.
- **DESCONOCIDO:** vendor function name, subsystem name, and semantic meaning of the backing state.

## Adversarial conclusions

- Rejected: treating `0x89909192` as a valid 2005 handler pointer.
- Rejected: `uint16_t`/`uint32_t` return width; all real callers consume the byte only.
- Rejected: implicit arguments; the complete handler reads none.
- Rejected: naming the backing byte as boolean/status/state simply because A3C0 normalizes it to 0/1.

Full ROM bytes, detailed caller contexts, corpus hashes, and reverse-engineering workpapers remain private in Drive.
