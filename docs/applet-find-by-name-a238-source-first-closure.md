# A238 / AppletFindByName — source-first closure

Status: **CLOSED A / SOURCE-FIRST**

This note records the restarted source-first audit of A238/index142. Historical SDK declarations, the current BetaWise header/stub, prior reconstruction notes, and neighboring SmartApplet functions were used only as correlation sources. The contract below was revalidated directly against the three canonical firmware images.

## Reconstructed ABI

```c
uint32_t AppletFindByName(const char *name, uint32_t start_index);
```

The older SDK documentation declared `uint8_t AppletFindByName(char *name, uint8_t start_index)`. Firmware shows that both the second argument and the returned runtime index are consumed/produced through full 32-bit data registers/stack slots; the const-correct 32-bit BetaWise declaration is therefore the faithful portable ABI.

## Confirmed behavior

A238 performs a **case-sensitive prefix search** through installed SmartApplet runtime slots.

1. `strlen(name)` is computed first. `name` is therefore required to be a valid NUL-terminated pointer; the handler performs no NULL check.
2. The raw 32-bit `start_index` is incremented before any slot is examined. The argument is thus **exclusive**: normal use with `start_index == 0` begins at runtime slot 1.
3. The search continues only while the candidate index is below 32.
4. Each candidate indexes the generation-specific `AppletHeader_t *` runtime table. Encountering a NULL table entry immediately returns 0; the routine does not skip holes.
5. The candidate name begins inline at `AppletHeader_t + 0x18`.
6. Firmware calls the internal implementation of `strncmp(candidate_name, name, strlen(name))`. Consequently, a shorter input is a prefix match; this is not an exact-name comparator. Comparison is case-sensitive.
7. A match returns the runtime index in `D0.L`; exhaustion/no installed candidate returns 0.

Normal runtime indices are 0..31. Because slot 0 is the System applet and 0 is also the not-found sentinel, callers cannot use the return value alone to distinguish those two meanings. The normal continuation convention avoids that ambiguity by starting with 0 and therefore searching from slot 1.

The raw handler does not narrow `start_index` to eight bits and has no explicit range check other than the post-increment `< 32` loop condition. Values outside the documented runtime range should therefore not be treated as validated inputs.

## Cross-ROM primary validation

Canonical images revalidated:

- AlphaSmart 3000, November 2005
- NEO, November 2005
- NEO/System 3.15, July 2013

A238 begins at `0x004E0DFA`, `0x005E3756`, and `0x00439B58`, respectively. A23C begins exactly `0x64` bytes later in every image, giving a 100-byte handler 3/3.

The implementations are instruction-for-instruction equivalent except for relocated helper/table addresses. The applet-header pointer-table bases are `0x0D82`, `0x0DDA`, and `0x0E0A`.

The first helper called is byte-identical across the three ROMs and implements `strlen`. The second helper is likewise byte-identical and implements the standard three-argument `strncmp` loop. Those helper identities are therefore established from primary code behavior, not just SDK naming.

Handler SHA-256:

- AS3000 2005: `13964c6f5a95ba7bf3326b1789be2b2cad954705ec5813e576287d26f1b76daa`
- NEO 2005: `1789ea42311de886eb596de4fa25942e21d43fd02ac5355dc394922383d762ec`
- NEO 2013: `490ef0cb8aed768a320a252cadfa8381e9b4246790d3f44ea6af2c8a95237940`

## Callers / xrefs

A renewed full-ROM search for direct absolute `JSR` calls to A238 found:

- AS3000 2005: 1, at `0x004DF864`
- NEO 2005: 1, at `0x005E21C0`
- NEO 2013: 2, at `0x0042ABE0` and `0x004385BE`

All four direct callers pass a zero `start_index`, so their first candidate is slot 1. The returned value is consumed as a nonzero/zero runtime-index result. This independently supports the exclusive-start and zero-sentinel interpretation.

## Hypothesis refutation

Primary firmware refutes several weaker descriptions:

- **not an exact-name lookup:** comparison length is `strlen(name)`, so prefixes match;
- **not an eight-bit raw ABI:** the handler loads the second stack argument as a longword and returns the runtime index through `D0.L`;
- **not inclusive of `start_index`:** the handler increments it before scanning;
- **not a sparse-table scan:** the first NULL header-table entry terminates the search;
- **not case-insensitive:** the helper is the bytewise `strncmp` implementation.

## Regression

Static source-first regression was **EXECUTED: 63/63 PASS** (21 checks per ROM). It covered canonical ROM identity, exact handler bounds, argument stack slots, `strlen` helper identity, full-width exclusive start-index handling, table base, 32-slot limit, NULL-slot termination, inline `+0x18` name field, `strncmp` helper identity/call order, prefix-match branch, index return, not-found return, direct-caller counts, and zero start-index at every direct caller.

Dynamic emulator/hardware regression remains **SPECIFIED / NOT EXECUTED**. Safe future cases include: exact installed name; unique prefix; nonmatching name; empty string; continuation using a previously returned index; and start index 31. Invalid pointers and deliberately out-of-range raw indices should be restricted to an emulator/sandbox.

## Confidence

- **CONFIRMED:** name field at `AppletHeader_t + 0x18`; case-sensitive prefix semantics; exclusive full-32-bit `start_index`; candidate range below 32; first-NULL termination; runtime-index return or 0; 100-byte handler 3/3; helpers `strlen` and `strncmp`; direct callers 1/1/2 all using start 0.
- **CONFIRMED ABI:** `uint32_t AppletFindByName(const char *name, uint32_t start_index)` for the reconstructed SDK.
- **HISTORICAL BUT SUPERSEDED:** `uint8_t AppletFindByName(char *name, uint8_t start_index)` from early SDK documentation.
