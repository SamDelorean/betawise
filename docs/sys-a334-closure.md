# A334 — raw blocking character input with echo

Status: **mechanically closed (A) / published raw contract**.

## Public ABI

```c
int32_t SYS_A334(void);
```

No independent vendor symbol name beyond `SYS_A334` has been recovered. BetaWise historically comments this trap as "getchar with echo"; that description matches the reconstructed mechanics but is not promoted to a vendor name.

## Mechanics

A334 is A-line index 205. The complete handler is 0x22 bytes in AS3000 2005, NEO 2005, and NEO 2013. It contains 15 decoded 68000 instructions, two external `BSR` calls, and a single final `RTS`. After relocation of one absolute data operand, all three handlers are byte-identical.

The first call resolves exactly to A330 / `_OS3K_getchar`. The returned character is preserved. A334 then calls A350 / `_OS3K_fputc` with that character and a static stream value of `1`, which is the SDK representation of `stdout`. The `_OS3K_fputc` result is discarded.

Immediately before return, A334 reconstructs the original character byte and applies `EXT.W` followed by `EXT.L`. Therefore the raw ABI result is the original 8-bit character **sign-extended** to the full D0 longword. This is materially different from A330, which zero-extends its byte result.

No separate EOF/status path is present in A334. Given the observed A330 returning domain of 1..255, A334's effective returned values are 1..127 and -128..-1 after the explicit sign extension. This closure does not assign the negative half of that domain an EOF meaning.

## Caller survey

The official SmartApplet corpus was exhausted with the corrected PC-index detector: all 30 applets containing the relevant A-line table were scanned, and the remaining 11 applets are structural negatives without that table. Executable A334 callers: **0/41**.

The absence of callers is not used to infer `void`. The handler itself deliberately defines D0.L from the original input byte after the echo operation, which establishes a mechanically intentional return value. A ROM-wide direct-address scan also found no absolute `JSR`/`JMP` callers outside the A-line path.

## Generational comparison

AS3000 2005, NEO 2005, and NEO 2013 are mechanically equivalent after relocation of the static `stdout` data reference. The relocation-normalized handler SHA-256 is `ed2c743d59e51c8a83a98e837e23817c9b67ee9ef88e006b9c5b93b3b5a3c240`.

## Adversarial conclusions

- The historical `void SYS_A334();` declaration is provisional and contradicted by the deliberate full-long return construction.
- The result of `_OS3K_fputc` is not the A334 return value; it is explicitly discarded.
- A334 is not renamed to `getche`, `getchar`, or another standard/nonstandard symbol without independent symbol evidence.
- Negative values produced by sign extension are not labeled `EOF`; no such semantic path was demonstrated.

Private workpapers retain ROM hashes, exact offsets and raw hashes, the 41/41 caller manifest, and the static regression.

Static regression: **OVERALL PASS**. Dynamic regression: **not executed**.
