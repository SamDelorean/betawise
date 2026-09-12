# SDK header reconciliation — batch 2

This note defines the next low-risk cleanup pass for `os3k/os3k.h` after the ABI consolidation through `A470`. It is intentionally limited to contracts already classified `CERRADA`; entries with `CERRADA_CON_LÍMITE_DE_EVIDENCIA` are not promoted to stronger C prototypes here.

## Build gate

The branch has no CI coverage for the recent SDK header changes. Before widening the public header further, validate the current state on the Linux/m68k toolchain:

```sh
cd ~/Projects/alphasmart/betawise
make -C os3k clean
make -C os3k
```

The build uses `m68k-elf-gcc` and the flags from `Makefile.common`. Until this succeeds, the current header state is `REVISADO_ESTÁTICAMENTE / COMPILACIÓN_PENDIENTE`, not `BUILD_VALIDATED`.

## Batch 2A — corrections to declarations already present

These are direct ABI corrections, not new semantic guesses:

```c
uint32_t SYS_A32C(void);  /* A32C/index203 */
int32_t  SYS_A334(void);  /* A334/index205 */
```

They replace the current historical declarations:

```c
void SYS_A32C();
void SYS_A334();
```

`A32C` has a confirmed full 32-bit return. `A334` returns the original character sign-extended in `D0.L`.

## Batch 2B — neutral raw contracts already closed

The following neutral names and mechanical signatures are sufficiently closed to expose without inventing vendor semantics:

```c
uint8_t  SYS_A0B8(void);
uint8_t  SYS_A0BC(void);
void     SYS_A0C0(uint8_t state);
uint8_t  SYS_A0C4(void);
uint16_t SYS_A0C8(void);
uint8_t  SYS_A0CC(void);
void     SYS_A0D0(uint8_t enabled);
```

The higher-level labels such as Auto Repeat, Sticky Keys, Slow Keys, or Show Marks remain inference where indicated in the private/source-first records. The public header should therefore keep the neutral `SYS_Axxx` names unless a historical source symbol is independently recovered.

## Batch 2C — NEO13-only closed contracts

These positions are callable in NEO13 and are not callable in AS3000/NEO05. They may be declared in a clearly marked NEO-only/raw section:

```c
uint16_t SYS_A3B4(void *record);
uint8_t  SYS_A3B8(uint16_t arg1, void *arg2, uint16_t arg3, uint8_t arg4);
uint8_t  SYS_A3BC(void);
uint8_t  SYS_A3C0(void);
uint8_t  SYS_A404(void);
void    *SYS_A41C(uint8_t selector);
char    *SYS_A438(char *dst);
uint16_t SYS_A470(void);
```

The names remain neutral because source-level/vendor identity is unknown, but the listed argument/return mechanics are closed. Do not add public C prototypes for `A3C4–A400`, `A408–A434`, or `A43C–A454` merely to complete the numerical sequence: those entries retain contractual limits.

## Late NEO callable helpers for emulator use

`A468` and `A46C` are mechanically closed, but their higher-level source identity is unknown. Their contracts are already sufficient for emulator implementation. They should remain documented first and can be promoted to SDK declarations only if a consumer needs them:

- `A468`: four physical slots; arg1 low byte, args2–4 full 32-bit; low-byte status `0/1/2`.
- `A46C`: one physical slot; selector low byte `0..10`; low-byte result.

This avoids prematurely freezing guessed C semantic types while preserving everything needed by the emulator.

## Compatibility rule

Raw trap names and compatibility wrappers must remain distinct. `GetModifierKeys`/`SetModifierKeys` are the canonical A0A0/A0AC veneers. Historical `GetKeyModifiers`/`SetKeyModifiers` are source-compatibility wrappers only.

## Acceptance criteria

A batch is considered integrated only when:

1. header declaration matches the consolidated ABI entry;
2. no `CERRADA_CON_LÍMITE_DE_EVIDENCIA` item is strengthened beyond evidence;
3. `make -C os3k` succeeds with the m68k toolchain;
4. at least one representative applet build succeeds after the library build;
5. the result is recorded in Control operativo, Bitácora, and the continuity handoff document.
