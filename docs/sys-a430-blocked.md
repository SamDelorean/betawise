# SYS_A430 — NEO 2013 — historical contract-limit note

Status: **superseded by source-first trace**.

This file preserves the earlier consolidation state in which A430 was described as “contract blocked.” The source-first audit reclassifies the unresolved semantic identity and public return type as **evidence limits, not implementation blockers**.

The current public record is [`abi-trace-a430.md`](abi-trace-a430.md).

The firmware still supports a strong procedure-like inference: multiple exits do not define D0, while loop execution merely inherits D0 from the last private helper call. That is not sufficient, by itself, to publish a callable `void` prototype without an independent source or caller.

Use the neutral `SYS_A430` name and keep the callable header withheld.
