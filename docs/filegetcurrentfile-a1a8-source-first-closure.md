# A1A8 / FileGetCurrentFile — source-first re-audit

Status: **closed A / source-first**.

```c
uint16_t FileGetCurrentFile(void);
```

## Source correlation

The early AS3000 `FileModule.c` contains `FileGetCurrentFile(void)` returning the active 1-based file number. This is a strong genealogical/name anchor, but not proof of the later System 3 return width or token model.

BetaWise already exposes `FileGetCurrentFile(void)` at A1A8/index106. Earlier File API reconstruction independently established the active-descriptor resolver and the modern canonical token namespace. Official SmartApplet callers previously identified in AlphaWord Plus, AlphaQuiz and KeyWords consume the return as a file token, independently supporting the modern identity.

## Canonical ROM revalidation

All canonical ROM SHA-256 values were rechecked before extraction.

| ROM | Runtime | File offset | Length |
| --- | ---: | ---: | ---: |
| AS3000 2005 | `0x004E16A2` | `0x216A2` | `0x36` |
| NEO 2005 | `0x005E3FFE` | `0x23FFE` | `0x36` |
| NEO 2013 | `0x0043A400` | `0x2A400` | `0x36` |

The three handlers share 50 of 54 bytes exactly. AS3000↔NEO2005 differs at three bytes, NEO2005↔NEO2013 at one byte, and AS3000↔NEO2013 at four bytes. Differences are resolver/diagnostic call displacement and trace metadata only.

## Firmware-confirmed contract

The handler takes no syscall arguments. It allocates a 16-bit local token slot, invokes the active-file resolver with token `0`, and receives both the descriptor pointer and canonical token output. If the descriptor resolves, the local token is zero-extended into the function result. If no descriptor resolves, the initialized zero result is retained.

Equivalent observable model:

```text
uint16_t FileGetCurrentFile(void)
{
    uint16_t token = 0;
    descriptor = resolve_file(0, &token);
    if (descriptor == NULL)
        return 0;
    return token;
}
```

No file storage, cursor, size, namespace, mirror binding, or active-descriptor state is mutated by A1A8 itself.

## Xrefs / callers

Each canonical ROM contains six absolute occurrences of the A1A8 handler address: one vector-table entry plus five direct `JSR abs.l` callers. The direct-caller count is therefore 5/5/5 inside System 3.

Earlier official SmartApplet analysis found ten external A-line callers across AlphaWord Plus, AlphaQuiz and KeyWords; several immediately zero-extend/use the returned word as a token for later File API calls. This corroborates the 16-bit return contract.

## ABI evolution / refutation

The early source returned an 8-bit 1-based file number. The modern handler demonstrably returns a 16-bit canonical token supplied by the System 3 resolver. Therefore:

- the name continuity `FileGetCurrentFile` is strongly supported;
- the historical 8-bit ABI must **not** be projected onto System 3;
- A1A8 is not a file-count getter, despite old provisional comments elsewhere;
- A1A8 is read-only, not an open/close/select primitive.

## Classification

- **CONFIRMADO:** no arguments; active resolver token 0; canonical 16-bit token result; zero when no active descriptor; no state mutation; 3/3 structural equivalence; five direct System 3 callers per ROM.
- **INFERENCIA FUERTE:** continuity of the historical vendor name `FileGetCurrentFile` into the modern ABI.
- **DESCONOCIDO:** whether the exact modern internal symbol retained the historical spelling, absent recoverable modern symbol data.

## Regression

Static regression **EXECUTED: 73/73 PASS** against the three canonical ROMs. It covers SHA-256, boundaries, local token construction, resolver call, null path, zero-extension of the 16-bit token, result path, vector entries, xref counts and cross-ROM equivalence.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**: no-active-file→0; open→stable token; repeated query non-mutating; close→0; cross-platform observable parity.
