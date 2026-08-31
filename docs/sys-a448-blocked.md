# SYS_A448 — XML-parser descriptor selection, source semantics blocked

Status: **MECANICA_RECONSTRUIDA A / BLOQUEADO_SEMANTICA_SELECTOR**.

`A448` is callable only in the NEO 2013 corpus examined here. The AS3000 2005 and NEO 2005 vector entries are non-pointer/unresolved values and are not treated as implementations of this service.

The handler has five 32-bit stack slots plus a contractual six-byte aggregate-result channel through `A1`.

Mechanically established roles are:

- slot 1: readable pointer to a descriptor-like object containing at least four 32-bit fields; A448 parses the bounded region described by fields at offsets 8 and 12;
- slot 2: pointer to a NUL-terminated comparison string. A predicate callback performs an exact byte-string comparison between this value and a parser-maintained NUL string;
- slot 3: writable base of an array of 16-byte descriptor records. Parser callbacks write four 32-bit fields at offsets 0, 4, 8, and 12 of each accepted record;
- slot 4: physically a 32-bit stack slot but consumed through its low byte and sign-extended. The parser compares produced-record count against this value with a signed byte relation, establishing an effective signed-8 capacity;
- slot 5: writable pointer receiving the produced/accepted record count as one byte.

The common parser used by A448 is demonstrably XML/markup-oriented at the behavioral level: its state machine explicitly recognizes `<`, `>`, `/`, `=`, double quote, and `&`, and uses the same entity-reference helper correlated independently in A444. A448 installs a predicate that returns true only when the parser-maintained NUL string equals slot 2. Accepted matches are materialized into the slot-3 descriptor array. This supports the neutral description “exact-string filtered XML parser descriptors”; the evidence does not yet distinguish whether the selected string is specifically a tag name, a path, or another parser key, so no stronger source-level name is assigned.

The incoming `A1` is preserved and receives an exact six-byte aggregate copy containing a state byte at offset 0, an uninitialized/padding-compatible byte at offset 1, and a 32-bit parser-progress/detail delta at offset 2. Both recovered direct firmware callers prepare a local six-byte destination in `A1`, call A448, inspect the aggregate state, inspect the produced-count byte, and later forward the aggregate. This confirms the aggregate-result channel at the binary ABI level.

A capacity-related state is mechanically constrained: the descriptor-writer callback sets state code 8 when the produced count is no longer less than the effective record capacity. Both recovered callers accept aggregate state 0 or 8 when at least one record was produced. This is sufficient to describe code 8 neutrally as a record-capacity condition; no vendor enum name is assigned.

`D0.L` contains the same 32-bit delta used to build the aggregate detail field at return. Unlike A440/A444, local control flow does not refute its coherence; however, both recovered callers ignore `D0` and consume the `A1` aggregate instead. A separate scalar-return contract is therefore not established and is not exposed.

## Resolution clues for consolidation

The remaining debt is source-semantic: determine the exact meaning/name of the four descriptor fields, determine whether slot 2 denotes an XML tag name, path, or another parser key, recover vendor meanings for remaining state values, and determine whether `D0` has any independent contractual role beyond being a temporary duplicate of the aggregate detail field. Useful evidence would include a historical prototype, a higher-level applet caller that reveals selector semantics, inspection of the immediately following A44C family wrapper and its callers, or dynamic XML probes that distinguish same-name elements at different nesting paths.

The second aggregate byte must remain padding/indeterminate-compatible. No callable public header or semantic function name is emitted while these source-level uncertainties remain.

Private ROM bytes, full correlated disassembly, parser callback analysis, hashes, and regression transcripts are retained outside the public repository.
