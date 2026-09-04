# SYS_A304 reconstruction closure

## Status

`A304` is mechanically closed with confidence A under the correlated-disassembly methodology. The recovered public ABI is:

```c
uint8_t SYS_A304(void *state, uint16_t value, const char *string);
```

The original vendor symbol and subsystem-specific names remain unknown.

## Evidence summary

The handler was independently bounded and decoded in the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware images. The AS3000 implementation is slightly shorter than the NEO implementations; NEO adds one preparation/helper call and the corresponding stack cleanup. NEO 2005 and NEO 2013 are otherwise mechanically equivalent after address relocation. The generational difference is preserved rather than normalized away.

Stack dataflow and official callers agree on exactly three physical 32-bit argument slots. `state` is a mutable firmware-owned structure. Only the low 16 bits of the second slot have externally observable effect, so the neutral declaration uses `uint16_t`. The third argument is a required NUL-terminated string that is compared bytewise after ASCII case normalization.

The success path updates a 16-bit field in `state` from `value`. The exact vendor meaning of that field, the searched collection, and the surrounding subsystem remain unknown and are intentionally not named here.

Every reachable exit defines the low byte of D0 as `0` or `1`. On the zero path the handler clears only `D0.B`, so the upper 24 bits are not contractual. Four executable official callers were found: two in AlphaWordPlus 2005 and two equivalent sites in AlphaWordPlus NEO 2012. All test the result as a byte. The public contract is therefore `uint8_t`, without assigning a vendor boolean or enum meaning.

## Caller survey

The official SmartApplet corpus was exhaustively checked: 30 binaries with A-line tables were rematerialized and hash-verified, while 11 applets without such a table were treated as structural negatives. Only the two AlphaWordPlus generations contain executable A304 callsites; the other 39 applets are negative. A separate ROM-wide direct JSR/JMP/BSR search found no direct caller in any of the three canonical ROMs.

## Adversarial checks

The reconstruction rejects several tempting overclaims. Apparent extra stack offsets resolve to the same three caller slots after accounting for internal pushes. The second argument is not promoted to 32-bit merely because it is initially loaded with `MOVE.L`; only its low word reaches observable effects. The return is not declared 32-bit because the zero path does not define upper D0 and real callers consume only `D0.B`. No subsystem or vendor terminology is inferred from neighboring syscalls or helper reuse.

## Validation

Private workpapers retain the per-ROM disassemblies, hashes, complete 41/41 caller manifest, correlated dataflow, helper references, generational comparison, and executable static regression. Static regression completed with `OVERALL PASS`. Dynamic/emulator-first regression remains specified but not executed and is not required for the mechanical closure stated here.

No firmware, ROM bytes, extensive disassembly, or other private vendor material is included in this repository document.

## 2026-09-04 source-first re-audit

The closure was re-audited under SOURCE-FIRST using the complete official SmartApplet caller corpus together with the previously reconstructed canonical firmware. The decisive independent correlation is four executable AlphaWordPlus callsites across two generations: two in AlphaWordPlus 2005 and two in AlphaWordPlus NEO 2012. Each call uses the same three-slot physical shape, and each consumes the returned low byte with `TST.B` before that register value is redefined. The remaining 39 official applets are documented negatives.

Firmware independently confirms the same three physical inputs, that only the low 16 bits of slot 2 reach an observable state update, and that slot 3 is dereferenced as a required NUL-terminated string. All reachable exits define `D0.B` as 0 or 1, while the zero path leaves the upper 24 bits non-contractual. This combination justifies the existing neutral `uint8_t` result without promoting it to a vendor `bool` or inventing enum semantics.

The real AS3000-versus-NEO implementation difference remains preserved: AS3000 has the shorter body, while NEO adds the preparation/helper sequence; NEO 2005 and NEO 2013 remain mechanically equivalent after relocation. No historical source recovered an independent vendor symbol or subsystem name, so `SYS_A304` and neutral field terminology remain appropriate.

Static source-first correlation regression: **15/15 PASS** (3 canonical ROM identity checks, 3 handler-length/terminal-CFG checks, 3 physical-slot/argument-use checks, 2 official caller-generation target sets, 2 explicit byte-result-consumption checks, and 2 complete-corpus/generational-difference checks). Dynamic/emulator regression remains **specified / not executed**.
