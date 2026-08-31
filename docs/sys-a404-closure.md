# SYS_A404 / index 257 — closure

Forward-sweep disposition for the OS3K A-line ABI.

## Contract

```c
uint8_t SYS_A404(void);
```

The name is intentionally neutral. No vendor symbol, boolean meaning, enum, or semantic field name has been recovered independently.

## Primary evidence

- AS3000 (2005): table value `0x5A06004E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2005): table value `0x7F08005E`, non-pointer/unresolved; no callable contract assigned.
- NEO (2013): runtime `0x004341FE`, file offset `0x0241FE`.
- Handler: `0x0C` bytes, three instructions, SHA-256 `a909402de74a82cc4556c04319727643ab86ea320b58244bf68762b4409d0b40`.
- Mechanical body: load a firmware-owned pointer from absolute state, read byte at offset `+0x111` into `D0.B`, return.
- Functional arguments: none. The handler does not read caller stack arguments.
- Exactly one direct NEO 2013 firmware JSR was found, at runtime `0x0043E382` / file `0x02E382`; direct JMP/BSR references are absent.
- Immediately after the JSR, that caller executes `MOVE.B D0,D7` and then compares `D7.B`. This positively establishes an 8-bit contractual return and refutes `void`, word, and longword return contracts.
- Upper 24 bits of D0 are not normalized by the handler and are non-contractual.
- The next A-line handler A408 starts contiguously at `0x0043420A` and writes a byte to the same private state offset, corroborating the getter/setter mechanical pairing without assigning semantic names.
- Official SmartApplet exposure is structurally absent from the validated late-table corpus; no independent public/vendor symbol or prototype was recovered.
- Static regression: `OVERALL PASS`.
- Dynamic/emulator-first regression: not executed.

Private ROM bytes, disassembly and detailed workpapers remain outside the repository.
