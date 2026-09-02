# A0F8 / index 62 — `DialogAddExitKey` source-first closure

Status: **CONFIRMED** for the raw ABI and core semantics in AS3000 2005, NEO 2005, and NEO 2013.

## Contract

```c
int DialogAddExitKey(Key_e key);
```

The handler consumes the low byte of the first compiler ABI slot. It does not validate, translate, or normalize the supplied key byte.

The dialog maintains an append-only exit-key byte array with capacity **15**. If the list is already full, A0F8 returns `-1` and makes no insertion. Otherwise it stores the supplied key at the current count, increments the count, and returns `0`.

Duplicate key bytes are not rejected by this syscall; mechanically they are appended like any other value while capacity remains.

## Source-first correlation

The function is the System 3 continuation of the historical `DialogMenuAddExitICode` role. Existing dialog-family reconstruction shows that `DialogRun` scans this exit-key list after shortcut processing. Thus a key can independently be a shortcut, an exit key, or both.

System code also uses the dialog family with concrete exit-key registrations, providing an independent caller anchor in addition to the public SDK declaration and syscall mapping.

## Three-generation firmware comparison

The canonical A0F8 handlers are exactly `0x26` bytes in all three generations. After masking only two relocated absolute-address operands, the bodies are byte-identical across AS3000 2005, NEO 2005, and NEO 2013.

The following are invariant across all three:

- byte-sized argument consumption;
- capacity constant `15`;
- full-list return `-1`;
- append at current count;
- count increment;
- success return `0`.

This directly closes the earlier generation-projection question for A0F8: NEO 2013 preserves the 2005 core ABI and capacity.

## Callers / xrefs

Direct absolute calls to A0F8 are present 7 times in each canonical ROM. The dialog runner consumes the list, and System dialog code provides concrete semantic callers.

## Regression

A private static/contract regression was **EXECUTED: 30/30 PASS**. It verifies canonical hashes, A0F8→A0FC boundaries, argument width/stack position, capacity and return paths, append/count behavior, and three-generation equality under a fixed two-field relocation mask.

Dynamic emulator/hardware regression remains **NOT EXECUTED**.

## Provenance / privacy

This document contains only an independently written functional contract and analysis. Firmware images, complete handler bytes, and extended disassembly remain private.