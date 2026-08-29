# A2E8 mechanical closure

Status: **mechanical confidence A**. Original vendor name and semantic enum names remain unknown.

## Neutral contract

```c
uint8_t SYS_A2E8(void *state,
                 uint32_t selector,
                 void *arg3,
                 uint32_t *out_value,
                 void *unused_arg5);
```

Five physical 32-bit stack slots are demonstrated by official callers. The handler itself reads only slots 1–4; slot 5 is preserved because it is physically present in the call ABI but unused by all three compared firmware implementations.

## Handler evidence

- AS3000 2005: runtime `0x004D4C0E`, file `+0x014C0E`, length `0x254`, raw SHA-256 `f7761a9cb63a3a6cd1b41984a88aa09c455a256175c01c48b5957a0c722fcbe7`.
- NEO 2005: runtime `0x005D6ED8`, file `+0x016ED8`, length `0x254`, raw SHA-256 `9e29269066412c8989a1c6eb2a64a20fe35bb6a3468e5d903fea2275ee9b994d`.
- NEO 2013: runtime `0x004285CE`, file `+0x0185CE`, length `0x25E`, raw SHA-256 `a2136926c3c34689db4fc6109c089f007464f72a385b7a68ddfd08ab94f58c00`.

AS3000 and NEO 2005 share the same core modulo relocation fields. NEO 2013 adds a real 10-byte preparation preamble before that core. Removing only that preamble for comparison and neutralizing the six relocation fields yields the common core SHA-256 `74b6107edf0cdf8fe9afcb28eef5deaeb751279a6d7b5e56c9eb11b5872cfa2d`.

Each handler has one terminal RTS. Private per-ROM disassemblies preserve the embedded dispatcher table as data rather than decoding it as instructions.

## Physical ABI

After the handler's register save, stack accesses recover:

1. `state` — mandatory mutable pointer;
2. `selector` — full 32-bit value;
3. `arg3` — caller pointer/structure;
4. `out_value` — mandatory `uint32_t *` output;
5. `unused_arg5` — present in official callers but never read by the handler.

The low byte of `selector` drives a table dispatcher, but the full selector is also compared against values including `0x00060001`, `0x0006000C`, and `0x00060020`; therefore the public contract does not narrow it to one byte.

`out_value` is cleared with a longword store before dispatch. Selected routes write additional longword values through it.

## Dispatcher and return

The compared core recognizes low-byte selector values:

`01, 07, 08, 0B, 0C, 0D, 10, 11, 12, 13, 1E, 1F, 20`.

No vendor enum names are inferred from those values.

A corrected 68000 PC-relative CFG audit shows every reachable exit converges through an explicit `MOVEQ #1,D0` or `MOVEQ #0,D0`. Thus the exact final mechanical domain is `D0.L in {0,1}`.

The neutral return type is `uint8_t`, not `bool`: official callers from both compared applet generations consume the low byte specifically, while no source establishes a vendor boolean type or name.

## Official caller evidence

The SmartApplet call idiom is indirect through the A-line table: `MOVEA.L #disp32,A0` followed by `JSR (-2,PC,A0.L)`. The target is reproducibly `file_offset(call)+6+sign_extend(disp32)`.

An exhaustive scan covered 41 official applets: 16 from the 2005 SmartApplets3000 corpus and 25 from the NEO 2012/2013 corpus. Thirty contain the A2E4/A2E8/A2EC table sequence. Exactly two executable calls resolve to A2E8, both AlphaWordPlus:

- AlphaWordPlus 2005, SHA-256 `22fc6a9b7ab96b332965074d81e88a61574c82bd118cd0a5122ff17eda348515`: A2E8 table word at file `+0x123DE`, call at `+0x0264`. The caller pushes five longwords, removes `0x14` bytes, copies D0 to D3, and later executes `TST.B D3` before redefining D3.
- AlphaWordPlus NEO 2012, SHA-256 `a8ae516734a0e0b2e19d51ffb7dff3893074ca733fa40207cbff7ead1433f29e`: table word at `+0x12EE6`, call at `+0x0252`. It uses the same five-slot shape, removes `0x14` bytes, copies D0 to D5, and consumes it with `TST.B D5` before redefining D5.

Observed argument shapes are equivalent to `[state_at_A5_plus_0xBA, D6, D7, A3, scratch_pointer]`. The fifth scratch pointer is not read by the handler.

Direct absolute JSR and direct BSR searches in the three canonical ROMs are negative; that is consistent with the applet-side indirect A-line mechanism and is not used as evidence of non-use.

## Internal graph and side effects

`state` accesses include offsets `+0x00`, `+0x02`, `+0x03`, `+0x1C`, `+0x1E`, indexed bytes from `+0x20`, and longwords at `+0x48`, `+0x4C`, and `+0x50`. `arg3` is accessed through longwords at `+0` and `+4`, with selected routes dereferencing the `+4` pointer.

A2E8 calls the already reconstructed A2C8 and A2EC paths and a private helper of length `0x1A0`. That helper is mechanically common across the three ROMs after seven relocation fields are neutralized, producing SHA-256 `46857f3e7b07a095d0637e8076821dca75e3ce46a0ec09e9f891b5b7ef5a7cdd`. Its internal BSRs resolve to A2C8, A2EC, and A2CC. A2E8 passes three pointers to it and ignores its D0 result. Remaining absolute helper names are deliberately left unknown.

NEO 2013's extra preparation calls are preserved as a genuine generational difference; they do not change the five-slot external ABI or final byte result.

## Adversarial review

- A previous four-argument hypothesis was refuted by two concrete official callers; the fifth physical slot is retained even though the handler ignores it.
- A previous non-binary-return interpretation was refuted after correcting the 68000 PC-relative branch base. Intermediate values such as 17, 8, and 19 are stored through `out_value`; they are not final returns.
- `selector` is not narrowed to `uint8_t` merely because dispatch uses its low byte.
- The return is not called `bool`; only its byte width and observed numeric domain are established.
- No semantic names are assigned to selector values, state fields, or anonymous helpers.

## Regression status

Emulator-first regression is **specified, not executed**. It should cover rejected/default selectors, every recognized selector route, final D0 byte values, `out_value` initialization and per-route writes, state mutations, and the NEO 2013 preparation preamble while comparing the common core across generations.

Private ROM bytes, extensive disassembly, helper listings, and the exhaustive applet manifest remain in Drive and are not published here.
