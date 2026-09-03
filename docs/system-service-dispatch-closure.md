# A25C global-service dispatcher — source-first closure

## Status

`A25C` is **CLOSED A / SOURCE-FIRST** for the AlphaSmart 3000 2005, NEO 2005 and NEO/System 3.15 2013 ROMs. No trustworthy original public/vendor symbol has been recovered, so the neutral name remains:

```c
int32_t SYS_A25C(uint32_t action_mask, KeyMod_e key);
```

“Global-service dispatcher” is a functional description, not recovered vendor nomenclature.

## Source-first correlation

Historical BetaWise/SDK material exposed A25C as a special-key helper with the provisional declaration `void SYS_A25C(uint8_t mask, KeyMod_e key)` and documented only bit 3 (`0x8`) as known. That declaration was useful as a locator, but it is not the modern firmware ABI.

Independent SmartApplet callers provide concrete usage before firmware interpretation: ControlPanel supplies action mask `8` with special-key words including `0xF012`, `0xF019` and `0xF01E`; AlphaQuiz and Calculator use mask `4` with key `0` in event/wait paths; ControlPanel also uses mask `1` with key `0`. GetKey uses A25C with mask `8` while preserving the original key value.

Official user/Manager documentation corroborates the existence of the surrounding user-visible special-key and communications/connection workflows, but it does not expose this internal syscall, its mask constants or its ABI. Those internal details therefore remain firmware-derived rather than being assigned names from UI terminology.

## Firmware-confirmed ABI

The three canonical handlers read the **entire first 32-bit argument slot** as `action_mask`, not a byte. Bits 0, 1, 2 and 3 are tested independently and in order, so combined masks request multiple service paths in one call.

`key` occupies the low word of the second 32-bit argument slot and is consumed only on the bit-3 route. The handler zero-extends that word before passing it to the private special-action helper.

The caller-visible signed 32-bit return is:

- `0` when the bit-3 route is not executed;
- `8` when the bit-3 helper reports recognition/success;
- `-9` when that helper does not report recognition; this path also calls a private housekeeping routine.

Therefore the historical `void` return and `uint8_t mask` declaration are both **refuted by primary firmware evidence**.

## Service paths

- **bit 0 (`0x1`)** — communications/service chain. AlphaSmart 3000 2005 and NEO 2005 execute a two-stage private chain, with the second stage reached only when the first returns zero. NEO 2013 adds a third conditional stage. Exact private subsystem names remain unknown.
- **bit 1 (`0x2`)** — one dedicated private service helper. Exact public meaning remains unknown.
- **bit 2 (`0x4`)** — one dedicated local service routine. Official SmartApplet callers use it in event/wait loops with `key == 0`; exact vendor name remains unknown.
- **bit 3 (`0x8`)** — special-key/action dispatcher. Concrete ControlPanel callers and GetKey independently corroborate this route.

The names of bits 0–2 are intentionally not promoted beyond these neutral functional descriptions.

## Cross-generation behavior

AlphaSmart 3000 2005 and NEO 2005 handlers are structurally equivalent and are `0x6C` bytes long. NEO 2013 preserves the ABI, bit ordering and return mapping but grows to `0x76` bytes because bit 0 gains an additional helper/test stage.

This is a real firmware-generation difference and must not be erased in an emulator or high-level model.

## Caller evidence

A complete direct absolute-JSR scan of each canonical ROM finds **3 / 3 / 3** native callers. All nine native sites prepare action mask `8` in the first argument slot and a 32-bit key slot, then remove eight argument bytes after the call. SmartApplet-linked caller analysis additionally demonstrates masks `1`, `4` and `8`.

This caller evidence agrees with the firmware-decoded two-slot ABI and disproves a special-key-only interpretation of A25C.

## Regression

Static source-first regression: **68 / 68 PASS — EXECUTED**.

It covers canonical ROM hashes, vector/index placement, runtime/file-offset conversion, handler bounds, 32-bit mask load, independent bit tests and ordering, low-word key path, return mapping, cross-generation bit-0 delta, and complete native direct-JSR caller counts/argument preparation.

Dynamic regression remains **SPECIFIED / NOT EXECUTED**. Dynamic tests should exercise masks `0`, `1`, `2`, `4`, `8`, combinations of bits, recognized and unrecognized bit-3 keys, and the NEO 2013 extra bit-0 stage where hardware/emulation permits.

## Safety and modeling guidance

A25C is not a pure query and must not be modeled as `void`. It advances or dispatches System 3 services. Firmware-first emulation should preserve the mask width, independent bit ordering, key low-word semantics, `0/8/-9` return behavior and the NEO 2013 bit-0 delta while leaving hardware-specific side effects firmware-driven whenever possible.

## Publication boundary

This document publishes the recovered contract and reproducible conclusions only. ROM bytes, extensive disassembly, complete private xrefs and binary workpapers remain outside the public repository.
