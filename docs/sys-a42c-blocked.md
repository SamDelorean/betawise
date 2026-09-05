# SYS_A42C — NEO 2013 — historical contract-limit note

Status: **superseded by source-first trace**.

This file records the earlier consolidation state in which A42C was described as “contract blocked.” The later source-first audit reclassifies the unresolved return type and semantic/vendor identity as **evidence limits, not implementation blockers**.

The current public record is [`abi-trace-a42c.md`](abi-trace-a42c.md).

The source-first revalidation also makes one mechanical detail explicit: the low byte of the second external slot is **sign-extended** before the `0x12C` stride multiplication. It should therefore not be summarized as a simple unsigned selector.

No callable header is published until independent evidence establishes the semantic contract and return type.
