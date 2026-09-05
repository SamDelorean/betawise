# A37C / `srand` closure

Status: **mechanical A / published**.

Reconstructed contract:

```c
void srand(unsigned int seed);
```

The three canonical OS3K ROMs resolve A37C/index223 to AS3000 2005 `0x004E7220` (`file+0x027220`), NEO 2005 `0x005E9568` (`file+0x029568`), and NEO 2013 `0x0044443C` (`file+0x03443C`). Each handler is exactly 0x0A bytes: it copies the complete 32-bit slot at entry `SP+4` to a fixed global and immediately returns. After neutralizing only that relocated absolute global address, all three handlers normalize to SHA-256 `22d695a1919cedcd0c727f7f87405e664fdc5e3ebc6d87272888281af27012f6`.

The destination global is exactly the same persistent PRNG state used by A370 / `rand`: AS3000 `0x0000041C`, NEO 2005 `0x00000418`, and NEO 2013 `0x00000440`. A370 reads that state, advances it with `state = state * 0x41C64E6D + 0x3039` modulo 2^32, writes it back, and returns the standard 15-bit result. A37C therefore sets the seed state verbatim; it performs no transformation, validation, range restriction, helper call, or secondary side effect.

A37C does not write `D0`. Preserved incoming `D0` contents are therefore not a constructed return value. The independently preserved BetaWise mapping identifies index223 as `srand`, and the existing public header already declares `void srand(unsigned int seed)`. Combined with the exact cross-ROM seed-state mechanics, this confirms the void C-library contract without interpreting residual register contents as a return.

The official SmartApplet caller sweep was rerun from the canonical binaries. All 30 table-bearing applets matched their preserved SHA-256 values and contained the expected C-library table once. The corrected PC-index detector finds zero executable A37C callers; the remaining 11 official applets are structural negatives, for a corpus result of 0/41. Detector blindness is excluded by exact same-run controls reproducing A36C=`printf` at 99 calls and A378=`sprintf` at 598 calls. Direct firmware absolute JSR/JMP and direct BSR xrefs to A37C are also negative in all three ROMs.

Adversarial alternatives are excluded mechanically. A37C is not a random-value getter because it never reads state or constructs `D0`; it is not a 16-bit seed setter because it copies the full longword slot; it is not a seed transform because the input is stored verbatim; and it is not a pointer setter because the argument is not dereferenced.

Private static regression against all three canonical ROMs and the complete official caller corpus executed with **OVERALL PASS**. Dynamic/emulator regression remains specified but was not executed. Firmware bytes and detailed disassembly remain private in Drive.

## SOURCE-FIRST re-audit — 2026-09-04

The sequential re-audit started from the independent historical API anchors: the BetaWise-generated syscall map assigns index223/A37C to `srand`, while `os3k.h` and `os3k.pdf` preserve `void srand(unsigned int seed)`. Those names and types were treated as hypotheses rather than proof. A370/`rand`, already revalidated in the current sweep, provided the independent state-machine anchor for the PRNG state.

Primary firmware was then re-extracted from all three canonical ROMs. Their SHA-256 identities, vector[223] targets, exact 0x0A handlers, raw handler hashes, relocated state-global operands, and normalized SHA-256 `22d695a1919cedcd0c727f7f87405e664fdc5e3ebc6d87272888281af27012f6` all reproduce independently. The handler copies the complete longword at entry `SP+4` verbatim to exactly the persistent state read and updated by A370. No helper, branch, validation, transform, dereference, or second write exists.

Return semantics were rechecked adversarially. A37C never constructs `D0`; residual incoming `D0` is therefore not evidence for a return value. With zero executable official callers, caller consumption cannot settle the source type, but the exact seed-setter mechanics plus the independent C-library prototype support `void srand(unsigned int seed)` and reject getter, 16-bit seed, pointer-setter, and transform interpretations. The canonical caller workpaper remains 0/41 with positive controls A36C=99 and A378=598; the known duplicate workpaper generated from an incorrect compact-table offset remains explicitly superseded and was not used.

Classification after re-audit: **CLOSED A / SOURCE_FIRST / PUBLISHED**. Previously executed static regression remains **OVERALL PASS**; dynamic/emulator-first regression remains **SPECIFIED / NOT EXECUTED**.
