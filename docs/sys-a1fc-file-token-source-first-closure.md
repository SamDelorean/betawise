# SYS_A1FC — independent source-first verification note

This file is retained solely to reconcile an overlapping audit run without rewriting repository history.

The canonical dedicated closure is:

- `docs/sys-a1fc-token-source-first-closure.md`
- originating commit `52aa69086060d2ef4ee91341f724151d587908f4`

This independent pass reached the **same contract and regression result** and therefore does not define a competing ABI description.

Verified independently:

```c
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);
```

- constructs `(token_high << 8) | token_low`;
- validates the constructed token through the common File API resolver;
- returns the complete token on successful resolution;
- otherwise propagates the resolver status;
- performs no descriptor mutation;
- preserves the same semantics across AS3000 2005, NEO 2005 and NEO 2013;
- direct-call correlation is `10/10/10`, with observed callers obtaining the effective high-byte group from `SYS_A254` immediately beforehand;
- static regression independently reproduced **59/59 PASS**;
- dynamic regression remains **SPECIFIED / NOT EXECUTED**.

No alternative vendor name is introduced. `SYS_A1FC` remains the neutral identifier.

For the normative source-first audit record, use the canonical closure named above. This note contains no ROM image, firmware bytes, or extended disassembly.
