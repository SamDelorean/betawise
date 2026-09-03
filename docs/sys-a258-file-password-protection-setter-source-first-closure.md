# SYS_A258 — file-password-protection state setter — source-first closure

Status: **CLOSED A / SOURCE-FIRST / PUBLISHED**

```c
void SYS_A258(uint8_t protection_state);
```

The original modern vendor symbol was not recovered, so the neutral `SYS_A258` name is retained.

## Correlation first

A258 was correlated with the immediately paired A248 getter, A1E0 `PasswordVerifyFileDialog`, A24C/A250 master-password gates, the previous password-runtime reconstruction, and the official NEO user documentation. The manual independently confirms that file passwords form an enable/disable protection feature and that individual file passwords cannot be configured until password protection is enabled. That documents the user-visible subsystem; it does **not** imply that this low-level setter itself performs authorization.

Firmware establishes the low-level contract directly.

## Canonical handlers

- AS3000 2005: runtime `0x004DFF62`, file `0x0001FF62`
- NEO 2005: runtime `0x005E28BE`, file `0x000228BE`
- NEO 2013: runtime `0x00438CBC`, file `0x00028CBC`

Each complete handler is exactly `0x0A` bytes. It consists solely of a byte move from the low byte of the first ABI slot to the shared protection-state global, followed by `RTS`.

Handler SHA-256:

- AS3000 2005: `4ea931c05fe92dd762a48a729cc888290337724197675270cb7495b0afbcc5c2`
- NEO 2005: `b2d3c410f8a043608f136ebeed90f1b758d56062cd47fffc35c8d2b026fbe7d2`
- NEO 2013: `dcb6f6e65c4ba678338fa5ff65addb28e4868eb51bd46b74513f03270278ab6c`

## Shared state with A248

The destination byte is exactly the byte returned by A248 in every canonical ROM:

| ROM | protection-state global |
| --- | ---: |
| AS3000 2005 | `0x00004472` |
| NEO 2005 | `0x000058A4` |
| NEO 2013 | `0x000118A2` |

A248/A258 are therefore a mechanically proven getter/setter pair.

## Confirmed semantics

A258:

- consumes the low byte of the first 32-bit ABI slot;
- writes that byte literally to the shared file-password-protection state;
- performs no validation or Boolean normalization;
- performs no master-password comparison or authorization;
- calls no helper and takes no branch;
- does not write D0, so stale D0 contents are not a return value and the public contract is `void`.

Normal System 3 flows use the state as 0/1, but the trap itself will store any byte value. Authorization belongs to higher-level flows such as A250, not to A258.

The official NEO documentation is consistent with this layering: it describes password protection as a user-controlled enabled state and separately describes master-password management. Firmware shows that the policy/UI layer and the raw storage primitive are distinct.

## Caller/xref review

Complete-ROM search found **0 / 0 / 0 direct absolute JSR references** to A258 in AS3000 2005 / NEO 2005 / NEO 2013. This negative result is preserved rather than treating absence of native direct callers as evidence against the ABI; A-line/SmartApplet invocation remains possible.

## Refutation

Primary evidence rejects the following alternatives:

- boolean-clamping setter — false; every low-byte value is stored literally;
- password-verifying setter — false; no comparison/helper/branch exists;
- combined getter/setter — false; there is only one byte store and no meaningful D0 result;
- per-file password setter — false; it writes the same global state byte read by A248, not any file descriptor password field;
- stable integer return — false; D0 is untouched.

## Classification

**CONFIRMED:** ABI; shared A248/A258 global; literal low-byte store; no validation; no authorization; no helper/branch; void return; 0/0/0 direct-JSR negative search; cross-generation equivalence.

**INFERENCIA FUERTE:** normal supported state domain is 0/1, based on higher-level firmware consumers and official user-visible enable/disable semantics.

**DESCONOCIDO:** original modern vendor symbol.

## Regression

Static primary regression was **EXECUTED: 30/30 PASS**. Ten checks per canonical ROM cover handler length, byte-store opcode, first-slot byte offset, exact destination global, equality with the A248 getter global, single-write structure, absence of helper call, absence of branch logic, final RTS, and complete-ROM negative direct-JSR search.

Dynamic regression is **SPECIFIED / NOT EXECUTED**: save A248 state; write 0 and 1 through A258; verify A248; optionally verify a non-Boolean byte is stored literally in an emulator; finally restore the saved state. Emulator-first execution is preferred because this mutates password-enforcement runtime state.

Firmware bytes and complete disassembly remain private.
