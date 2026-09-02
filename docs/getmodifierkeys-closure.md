# A0A0 / index 40 — modifier-key getter closure

## Status

**MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST / 54/54 PASS / COMPLETADO**

A0A0 is the raw getter for the current 16-bit modifier-key state:

```c
KeyMod_e GetModifierKeys(void);
```

The contractual return lives in `D0.w`. The upper word of `D0` is not normalized or guaranteed.

## Source-first correction

`os3k/syscall.c` assigns index 40 to `GetModifierKeys`. The historical public header instead declares:

```c
uint8_t GetKeyModifiers(void);
```

and says that it returns `KeyMod_e` shifted right by eight bits. That header declaration does **not** describe the raw A0A0 firmware ABI. It may only be retained as an explicitly implemented compatibility wrapper that calls the raw trap and performs the shift.

## Canonical ROM evidence

| Generation | Handler | File offset | Modifier word | Raw SHA-256 |
|---|---:|---:|---:|---|
| AS3000 2005 | `0x004CDF5E` | `0x00DF5E` | `0x0000432C` | `2836508c043ae291c593e63f00a00e09960cfbda7cffa0183f3b242d7636512b` |
| NEO 2005 | `0x005D1F42` | `0x011F42` | `0x000057B6` | `ddbf114c7d72eb511cd0344698c44e19857a00306cfe59311b3ab48c601cf7e4` |
| NEO 2013 | `0x00422F32` | `0x012F32` | `0x00005D0E` | `f6eba5dd866a762909e03a0c9f115b9f5e8ab79caa377c68f6ed3307420a56c0` |

The full ROM SHA-256 values were revalidated against the canonical images:

- AS3000 2005: `732dca63399d883071ce41cc07ad352fa19c506b7b8c307ace16d5c53e6f8708`
- NEO 2005: `5f550e48ad36892ee5f5a067e8c1b87f6bf504b6cb2c2dfc4098776fa8831a3d`
- NEO 2013: `32fe55bb50c1b58326bd303f14973b8402ddf8bf6662b210e4d5e9f931c7e6c0`

Each handler is exactly eight bytes:

```asm
MOVE.W  modifier_word,D0
RTS
```

After neutralizing only the absolute address of `modifier_word`, all three handlers are byte-identical. Normalized SHA-256:

`eedb3cd56bcc98e82ffae3b68085ecb32194edbc2c17ae2bff816daa91725e47`

A0AC / index 43 (`SetModifierKeys`) independently confirms the same storage by executing `MOVE.W 6(SP),modifier_word; RTS` in all three generations.

## Official callers

A structural scan covered 40 official applets from the 2005 and 2012 distributions. It resolved 20 executable A0A0 xrefs in eight files:

| Applet | Generation | Xrefs |
|---|---:|---:|
| ControlPanel | 2005 | 1 |
| KeyWordsWireless | 2012 | 1 |
| AlphaQuiz | 2012 | 1 |
| ControlPanel | 2012 | 2 |
| KeyWords | 2012 | 1 |
| Responder | 2012 | 6 |
| WirelessFileTransfer | 2012 | 7 |
| WirelessUpdate | 2012 | 1 |

These callers explicitly consume high modifier bits using operations including `BTST #9`, `BTST #14`, `ANDI.W #0xC000`, and `ANDI.W #0xFC00`. This independently refutes an eight-bit or right-shifted raw return contract.

## Contract and limits

- No arguments.
- Reads the current modifier-state word.
- Returns the unshifted `KeyMod_e` mask in `D0.w`.
- Does not validate or transform the state.
- Does not scan the keyboard, consume a queued key, or mutate the modifier word.
- Because the handler writes only `D0.w`, callers must not rely on the upper 16 bits of `D0`.

## Regression

The static/contractual regression completed **54/54 PASS** with exit status 0.

- source SHA-256: `5b209d7e050acd9d2da988d4f3abf18f4b71d3ba80086a07c9f85d591389c6dd`
- output SHA-256: `1edad19d022dcf3a1f82d8aabf746c37584276f76e488694da637970c092d241`

This is a static ROM/flow/caller regression, not a dynamic 68k probe.

## Disposition

Document A0A0 as the raw `GetModifierKeys` trap returning `KeyMod_e`. Do not publish `uint8_t GetKeyModifiers(void)` as the native A0A0 contract. A compatibility wrapper may preserve that historical API only if its right shift is implemented explicitly outside the trap.
