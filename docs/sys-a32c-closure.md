# A32C — neutral 32-bit global-state getter

Status: **mechanically closed (A) / published contract**.

## Public ABI

```c
uint32_t SYS_A32C(void);
```

A32C consumes no caller arguments. In all three canonical firmware generations its complete handler is eight bytes and performs one full-long load from a firmware global into D0 followed immediately by `RTS`. The 32-bit value in D0 is therefore the sole observable result of the service rather than incidental staging.

The implementation is mechanically identical between AS3000 2005, NEO 2005, and NEO 2013 after relocating the source global. There are no branches, helpers, or other side effects in the handler.

## Caller survey

The official SmartApplet corpus was exhausted. All 30 applets with the relevant A-line table materialize A32C directly after A308 (the null A30C–A328 run is omitted from those tables), but no executable A32C caller was found. The remaining 11 applets are structural negatives without an A-line table. Result: **0 executable callers / 41 official applets**. Direct ROM-wide JSR/JMP/BSR references to the handler are also negative.

Caller absence does not make the return ambiguous here: the entire function is the global-to-D0 getter and has no competing effect.

## Meaning of the global

The source global is intentionally left unnamed. Cross-ROM writers repeatedly store `0x22`; equivalent paths inside the firmware implementation of `sscanf` perform signed numeric range checks immediately before those writes. NEO firmware also contains a writer of `0x0c` on a pointer-allocation failure path. These values correspond to the conventional C error numbers ERANGE (34) and ENOMEM (12), and A32C precedes a dense run of standard-C-library A-line services.

Together these observations make an `errno`-like interpretation a **strong inference**, but no independent original OS3K/vendor symbol has been recovered. This reconstruction therefore keeps the neutral name `SYS_A32C` and a neutral `uint32_t` result instead of asserting an `errno` API name or signed C type.

## Adversarial conclusions

- The historical provisional `void SYS_A32C()` declaration is inconsistent with the complete getter-only handler and is not retained as the reconstructed ABI.
- A pointer/handle interpretation lacks positive support and is disfavored by the small integer values written by multiple library paths.
- The semantic label `errno` is not promoted to confirmed vendor nomenclature without an independent symbol or source declaration.
- No dynamic test is claimed.

Private workpapers retain canonical ROM hashes, exact offsets/bytes, raw and relocation-normalized handler hashes, global xrefs, official-corpus scan evidence, and the executed static regression.

Static regression: **OVERALL PASS**. Dynamic regression: **not executed**.
