# A2F4 mechanical closure

Status: **mechanical confidence A**. The original vendor function name, `arg4` meaning, output-value meaning, helper names, and symbolic result names remain unknown.

## Neutral contract

```c
uint32_t SYS_A2F4(void *state,
                  uint32_t message,
                  uint32_t param,
                  void *arg4,
                  uint32_t *out_value);
```

Official callers construct exactly five physical 32-bit slots and remove `0x14` bytes after the call. `out_value` must be writable: the handler clears it on entry and writes a longword on an observed route. `arg4` is intentionally opaque; official callers pass pointer values and the handler forwards it downstream.

`D0.L` is contractual. An official caller executes `TST.L D0` immediately after stack cleanup, and the handler epilogue explicitly produces raw values `1` or `0`. No public symbolic names are assigned to those values.

## Handler evidence

- AS3000 2005: runtime `0x004DA3A2`, file `+0x01A3A2`, length `0x386`, raw SHA-256 `d03c6214b16c600e3d52341b73a93998df083c62185b1eb463c7e866167d6b73`.
- NEO 2005: runtime `0x005DCAE2`, file `+0x01CAE2`, length `0x3CA`, raw SHA-256 `a0210015bcb18e69e0a231faa56e4e30755df8029613544a265c11db90ca5ce3`.
- NEO 2013: runtime `0x0042F200`, file `+0x01F200`, length `0x4A8`, raw SHA-256 `3152b359904ca5f9b121c7bfa0aa22a7b23889ac7af9a3a97dd08c384b0d4fbb`.

A reproducible 68000 CFG pass reports 284 / 298 / 359 instructions, zero unknown opcodes, one final reachable `RTS` at `+0x384 / +0x3C8 / +0x4A6`, no indirect `JMP`, and all direct non-BSR branch targets internal and instruction-aligned.

## Physical ABI

After register/local setup, the handler loads the five incoming slots into the equivalent of `A4,D6,D7,A6,A3`. Correlated official callers establish the order as `state,message,param,arg4,out_value`.

`message` is compared as a 32-bit value against concrete values including `0x1B`, `0x20`, and `0x21`; `param` is consumed at longword/word/byte granularity and contains event/key/modifier information. These observations do not justify inventing additional enums in this closure.

## State, dispatch, and helper graph

Common state offsets used across generations include `+0x02`, `+0x0E`, `+0x12`, `+0x1A`, `+0x24`, `+0x2A`, `+0x2C`, and `+0x40`; NEO additionally uses `+0x16`. Field names remain unknown.

A word at `state+0x40` selects an indirect handler from a firmware pointer table. The table bases differ by ROM (`0x004EAA0E`, `0x005F1338`, `0x0044C8F0`) while the dispatch mechanism is homologous. A2F4 forwards five slots to the selected handler and tests its returned D0.

The body also invokes private helpers for state update/normalization, cursor/position transformations, and event processing. Their mechanics are recorded in private correlated workpapers; public names are not inferred.

## Official caller corpus

The A-line caller detector was corrected adversarially for `JSR d8(PC,A0.L)`: the PC base is the extension-word address, not the subsequent PC. The earlier +2 interpretation is superseded.

The corrected exhaustive scan covers **41 official SmartApplets**. Thirty applets containing the A-line table were rematerialized, SHA-256 checked against the canonical manifest, and their target table word verified to be literal `A2F4` before scanning. Eleven table-less applets are structural negatives.

Exactly two applets contain A2F4 calls:

- AlphaWordPlus 2005, SHA-256 `22fc6a9b7ab96b332965074d81e88a61574c82bd118cd0a5122ff17eda348515`: five calls at `+0x0B86`, `+0x13F6`, `+0x2292`, `+0x115F6`, `+0x11636`.
- AlphaWordPlus NEO 2012, SHA-256 `a8ae516734a0e0b2e19d51ffb7dff3893074ca733fa40207cbff7ead1433f29e`: five calls at `+0x0F8C`, `+0x18C6`, `+0x25A0`, `+0x120F6`, `+0x12136`.

The other 39 official applets are negative. The principal caller pair constructs `state,D6,D7,A3,&local`, removes `0x14`, then tests D0. Other pairs independently show concrete `message=0x1B/0x21` and `param=0x0D/0x4B` values and pointer-valued fourth/fifth slots.

## Generational comparison

All three ROMs preserve the five-slot ABI, message/param roles, indirect dispatch through `state+0x40`, writable fifth slot, and explicit `0/1` D0 epilogue. NEO uses a larger local frame and adds state field `+0x16`; NEO 2013 extends internal routes further. These differences are retained without normalizing private implementation details.

## Adversarial review

- The historical “two explicit arguments plus three pre-existing contextual slots” model is **rejected**; it arose solely from the +2 PC-index bug in caller resolution.
- The A-line dispatcher does not inject arguments; it removes the exception frame and transfers control to the handler.
- D0 is not incidental residue: it is explicitly defined by the handler and consumed by an official caller.
- The five-slot order is supported independently by handler loads and caller pushes/cleanup.
- No vendor name, status enum, helper name, or unproved field semantics are introduced.

## Regression status

A corrected private static regression was **executed and passed**. It revalidates canonical ROM hashes, handler hashes/lengths/CFG, the corrected 41/41 caller corpus, five-slot stack shape, `0x14` cleanup, contractual D0, state-offset set, indirect dispatch table, and generational differences.

Dynamic/emulator regression is not claimed as executed. Firmware bytes, extensive disassembly, detailed helper workpapers, and exhaustive private manifests remain in Drive.
