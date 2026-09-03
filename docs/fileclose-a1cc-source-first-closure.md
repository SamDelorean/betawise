# A1CC / FileClose — source-first closure

Status: mechanically and semantically closed against the three canonical System 3 firmware generations. `FileClose` is retained as the inherited BetaWise name; an independent original vendor symbol has not been recovered.

## Contract

```c
void FileClose(void);
```

The public handler has one direct effect:

```text
active_descriptor = NULL
```

It consumes no stack arguments, calls no helpers, dereferences no descriptor, and constructs no contractual return value.

## What it does not do

A1CC does **not** itself erase or copy storage, change current/max/recoverable size, move the cursor, synchronize live mirrors, rewind the file, or invoke a persistence/flush helper.

This matters when comparing terminology. `neo-re` models A1CC as `commit_editable_buffer` from higher-level caller transaction patterns. That may describe the protocol effect of ending an edit session in those callers, but the handler itself performs no material data commit; mechanically it only detaches the active descriptor.

## Evidence summary

The handler is exactly 8 bytes in AlphaSmart 3000 System 3 (2005), NEO System 3 (2005), and NEO System 3 (2013): `CLR.L <relocated active_descriptor>; RTS`. The target global is the same one to which A1C8 writes its newly resolved descriptor.

There are **20 / 20 / 20** direct absolute JSR callers in the complete firmware images, plus the direct BSR from A1C8's successful-open path.

## Confidence

- **CONFIRMED:** `void(void)` ABI; clear of the active-descriptor global; no helper or descriptor mutation; equivalent semantics across all three canonical firmware generations.
- **STRONG INFERENCE:** inherited name `FileClose` and role as active-context detach/close.
- **PROVISIONAL / caller-contextual:** `commit_editable_buffer` as a description of a higher-level transaction, not as the handler's direct implementation.
- **UNKNOWN:** original vendor symbol.

Static regression against the three canonical images: **30/30 PASS**. Dynamic/emulator verification is specified separately and is not claimed as executed here.
