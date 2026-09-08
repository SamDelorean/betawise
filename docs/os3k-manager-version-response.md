# NEO OS3K Manager VERSION response

This note records a functional reconstruction of the NEO 2013 System 3 Manager protocol. It does not contain firmware bytes or a firmware disassembly.

## External protocol contract

Public AlphaSync/neotools-compatible protocol definitions identify:

- request `0x00` as `REQUEST_VERSION`;
- response `0x40` as `RESPONSE_VERSION`;
- the response payload as a 64-byte revision record;
- revision major at payload offset `+0x04`;
- revision minor at `+0x05`;
- product name at `+0x06`;
- build date at `+0x19`.

As a cross-check on the command selector, request `0x08` is `REQUEST_RESTART` and its response is `0x52`.

## NEO 2013 firmware behavior

For the canonical NEO 2013 image, the internal Manager request dispatcher uses the request command byte as an index into a command jump table.

The `0x00` case:

1. selects the revision record stored at the end of the main OS segment;
2. exposes exactly `0x40` bytes of that record as payload;
3. constructs response code `0x40` with payload length `0x40`;
4. reports a total transfer length of `0x48` bytes: the 8-byte protocol message plus the 64-byte revision payload.

The `0x08` case independently produces response `0x52`, matching the public protocol definition and providing a cross-check that the jump-table selector is the Manager command number rather than an unrelated local enum.

## Structural consequence

NEO 2013 contains two byte-identical copies of the 0x80-byte entry/metadata trailer inside the main OS image. The Manager VERSION path selects the revision-record subobject from the copy located at the physical end of the main OS segment, rather than the earlier copy at the legacy 0x40000 boundary.

Therefore the repeated revision metadata is functionally live: it is the source returned by `REQUEST_VERSION`. This explains the functional need for the repeated **revision-record subobject**. It does **not** by itself establish why the preceding entrypoint/scaffold half of the 0x80-byte trailer is also duplicated, so that architectural detail remains unresolved.

## Confidence

- Manager command dispatch role: **confirmed**.
- `0x00` -> VERSION response contract: **confirmed**.
- terminal revision record as VERSION payload source: **confirmed**.
- reason the complete 0x80-byte trailer, rather than only the revision record, is duplicated: **unresolved / insufficient evidence**.

No A-line ABI entry is implied by this reconstruction. The demonstrated A-line frontier remains separate from this internal Manager protocol code.
