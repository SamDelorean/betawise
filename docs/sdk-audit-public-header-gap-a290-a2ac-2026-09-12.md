# SDK audit — public umbrella-header gap A290–A2AC

Status: **AUDIT_FINDING / NEXT_BUILD_BATCH**

## Finding

The current `sdk/abi-automation` branch contains focused public headers and existing trap veneers for the eight mechanically reconstructed contracts A290, A294, A298, A29C, A2A0, A2A4, A2A8 and A2AC, while the umbrella header `os3k/os3k.h` does not expose them.

This is a public-SDK consistency gap, not unfinished reverse engineering.

## Focused contracts already present

```c
/* os3k/sys_a290.h */
typedef struct __attribute__((packed)) {
    uint32_t field_0;
    uint8_t field_4;
    uint8_t field_5;
} SYS_A290_Record;
uint8_t SYS_A290(SYS_A290_Record records[6], uint8_t optional_out32[32]);

/* os3k/sys_a294.h */
uint8_t SYS_A294(uint32_t value);

/* os3k/sys_a298.h */
uint8_t SYS_A298(uint8_t selector, uint8_t value);

/* os3k/sys_a29c.h */
uint8_t SYS_A29C(void);

/* os3k/sys_a2a0.h */
uint8_t SYS_A2A0(uint8_t selector);

/* os3k/sys_a2a4.h */
uint8_t SYS_A2A4(uint8_t flags,
                 uint8_t selector,
                 const uint8_t *payload,
                 uint8_t payload_size);

/* os3k/sys_a2a8.h */
uint8_t SYS_A2A8(uint32_t context,
                 const uint8_t *payload,
                 uint8_t payload_size);

/* os3k/sys_a2ac.h */
int32_t SYS_A2AC(uint8_t *out_byte);
```

The focused headers deliberately retain neutral A-line names because vendor identities and some field/selector semantics remain unknown, but the callable C shapes are already reconstructed.

## Veneer confirmation

`os3k/syscall.c` already emits the corresponding veneers at indices 164–171:

```text
164 A290
165 A294
166 A298
167 A29C
168 A2A0
169 A2A4
170 A2A8
171 A2AC
```

Therefore applications that know the focused-header layout can already link these traps; applications that include only `os3k.h` cannot name them safely.

## Scope boundary

Do not infer that every numerically adjacent trap after A2AC should be promoted in the same batch. A2B0 and later entries must be gated by their own consolidated state; several later contracts are explicitly `CERRADA_CON_LÍMITE_DE_EVIDENCIA` and must not be strengthened merely for numerical continuity.

A290–A2AC are a suitable standalone umbrella-header batch because each already has a focused header with an explicit C contract and a matching veneer.

## Recommended integration pattern

Prefer including the focused headers from `os3k.h` rather than duplicating all declarations manually. This is especially useful for A290 because its packed record typedef is part of the public contract and should have one canonical definition.

If direct declarations are used instead, they must remain byte-for-byte type-compatible with the focused headers.

## Build gate

Before marking the umbrella reconciliation complete:

1. expose only A290–A2AC from their existing focused headers;
2. `make -C os3k clean all` with the real m68k toolchain;
3. build/link at least one representative applet including only `os3k.h`;
4. compile a small translation unit that includes both `os3k.h` and each focused header to detect typedef/prototype conflicts;
5. record the result as `BUILD_VALIDATED` only after those checks pass.

No ABI inventory count changes. No vendor names are assigned. No runtime test is claimed by this audit note.
