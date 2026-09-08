# OS3K Manager VERSION response

This note records a functional reconstruction of the System 3 Manager VERSION path across AlphaSmart 3000 (2005), NEO (2005), and NEO (2013). It does not contain firmware bytes or a firmware disassembly.

## External protocol contract

Public AlphaSync/neotools-compatible protocol definitions identify:

- request `0x00` as `REQUEST_VERSION`;
- response `0x40` as `RESPONSE_VERSION`;
- the response payload as a 64-byte revision record;
- revision major at payload offset `+0x04`;
- revision minor at `+0x05`;
- product name at `+0x06`;
- build date at `+0x19`.

As an independent selector cross-check, request `0x08` is `REQUEST_RESTART` and its response is `0x52`.

## Manager request namespace

The public protocol sources define a continuous 32-selector request namespace, `0x00` through `0x1f`. The firmware-side Manager dispatcher has the same physical width: a 32-entry indexed table. NEO 2013 additionally verifies the command-domain guard before that table, while the earlier canonical generations retain the homologous indexed-dispatch pattern.

This is a protocol namespace, not an A-line syscall table. Functional names are retained only where supported by public protocol evidence and, where claimed as a firmware contract, by a matching firmware branch.

Examples include VERSION (`0x00`), BLOCK_WRITE (`0x02`), LIST_APPLETS (`0x04`), REMOVE_APPLET (`0x05`), WRITE_APPLET (`0x06`), RESTART (`0x08`), settings/applet/file operations, and the updater controls at `0x16` through `0x18`. Requests `0x01`, `0x03`, `0x0a`, and `0x19` remain semantically unknown in the public protocol sources.

One source-history correction is worth preserving: the older AlphaSync header omitted a request definition for selector `0x05`, but later neotools identifies it as REMOVE_APPLET, and neo-re independently emits the corresponding request as command `0x05` with the established `(5, index)` arguments. Selector `0x05` therefore must not be treated as an unused hole merely because one historical header omitted the define.

## Cross-generation structural invariant

The canonical images declare these primary OS segments:

| Generation | Main OS segment | Segment size | VERSION revision record |
|---|---:|---:|---:|
| AlphaSmart 3000 (2005) | `0x004C0000` | `0x40000` | `0x004FFFC0` |
| NEO (2005) | `0x005C0000` | `0x40000` | `0x005FFFC0` |
| NEO (2013) | `0x00410000` | `0x60000` | `0x0046FFC0` |

In all three generations, the revision record selected by Manager request `0x00` begins exactly 64 bytes before the end of the declared main OS segment.

The three Manager dispatchers are homologous at the relevant control-flow level. Their request tables route command `0x00` to the VERSION path and command `0x08` to the RESTART path. In each VERSION case, the firmware materializes the terminal revision-record address, constructs a `0x40`-byte VERSION payload, and reports a total transfer length of `0x48` bytes. In each RESTART case, the response selector is `0x52`.

This establishes a stable architectural rule:

> Manager VERSION reads the 64-byte revision record at the physical end of the declared main OS segment.

## Consequence for NEO 2013

Earlier System 3 images use a `0x40000`-byte main OS segment, so their legacy trailer at the `0x3FF80` boundary is also the segment-terminal trailer and its revision record is the VERSION source.

NEO 2013 expands the main OS segment to `0x60000` bytes. It retains an earlier trailer copy at the legacy `0x40000` boundary and places a byte-identical trailer at the new physical end of the main segment. The Manager VERSION path selects the revision record from this terminal copy.

Therefore the repeated NEO 2013 revision metadata is not evidence, by itself, of a firmware bank, fallback image, or recovery image. Its revision-record half preserves the older cross-generation rule that VERSION metadata lives at the end of the main OS segment.

This explains the functional need for the repeated **revision-record subobject**. It still does not establish why the preceding 64-byte entrypoint/scaffold half of the 128-byte trailer is duplicated, so that detail remains unresolved.

## Verification status

A private reproducible regression covering the three canonical images checks image identity, main-segment geometry, terminal revision-record placement, the VERSION dispatcher path, payload sizing, and the RESTART control case. The completed run passed all 61 checks. A separate NEO 2013 dispatcher regression passed all 26 checks and directly verifies the 32-entry dispatch geometry and command-domain guard.

Confidence:

- Manager command dispatch role: **confirmed**.
- 32-selector Manager request namespace as the dispatch domain: **confirmed as a structural/protocol correlation**; individual branch contracts are not implied by this statement.
- `0x00` -> VERSION response contract: **confirmed**.
- terminal revision record as VERSION payload source in AS3000, NEO 2005, and NEO 2013: **confirmed**.
- cross-generation terminal-metadata invariant: **confirmed**.
- reason the complete NEO 2013 128-byte trailer, rather than only its revision record, is duplicated: **unresolved / insufficient evidence**.

No A-line ABI entry is implied by this reconstruction. The demonstrated A-line frontier remains separate from this internal Manager protocol code.