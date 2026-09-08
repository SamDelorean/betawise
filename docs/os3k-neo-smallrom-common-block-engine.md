# NEO Small ROM common block engine

This note records a source-first reconstruction of the block-transfer engine used by the NEO Small ROM while updating the main OS. It is intentionally separate from the A-line ABI: the command selectors documented here belong to the Manager/update protocol and their physical handler locations must not be interpreted as A-line syscalls.

## Source-first context

Current `alpha-core`/`neo-re` updater logic and older AlphaSync-derived protocol definitions show the following high-level transaction for a full NEO OS update:

```text
principal OS:
    0x18 -> 0x56        enter Small ROM context

Small ROM:
    0x16 -> 0x54        clear destination segment map
    0x17 -> 0x55        register/erase each destination segment

    [
      0x02(len,sum16) -> 0x42
      raw block bytes  -> 0x43
      0x0B             -> 0x47
    ] * N

    0x07 -> 0x48        finalize active transaction
```

The per-block `0x02/raw/0x0B` sequence and final `0x07` are the same host-visible envelope used during SmartApplet installation in the principal OS. The destination-preparation stage is different: SmartApplet installation uses its principal-OS setup path, while the Small ROM uses `0x16/0x17` and a segment mapper.

That correlation suggested a `DESTINATION_SETUP + COMMON_BLOCK_ENGINE` model. The canonical Small ROM now confirms the model directly.

## Canonical Small ROM

The image distributed with NEO Manager 3.9.3 is 0x581A bytes and has SHA-256:

```text
6402c40878ba28747642ce474361b1596555b2a1f137db79c7ea8bb5b35a9523
```

The physical command dispatcher contains the selector list:

```text
00 02 07 08 09 0A 0B 16 17 18 19
```

Its displacement table is traversed in reverse index order. Direct decoding resolves these protocol commands to the following file offsets:

| Selector | Physical handler offset |
| --- | ---: |
| `0x00` | `0x1D1A` |
| `0x02` | `0x1D6A` |
| `0x07` | `0x1F8C` |
| `0x08` | `0x1F78` |
| `0x09` | `0x1FC2` |
| `0x0A` | `0x1DA2` |
| `0x0B` | `0x1DB4` |
| `0x16` | `0x1C10` |
| `0x17` | `0x1C24` |
| `0x18` | `0x1C04` |
| `0x19` | `0x1FA0` |

These offsets are physical objects in the Small ROM, not A-line entries.

## `0x02`: arm raw block reception

The `0x02` handler at file offset `0x1D6A` establishes the state for the next raw block. Mechanically it:

- clears the running 16-bit checksum;
- latches the host-supplied expected `sum16`;
- latches the announced block length;
- computes the end cursor from the current staging cursor plus the block length;
- installs a dedicated raw-receive callback;
- prepares response `0x42`.

This confirms that `0x02` is a generic block-announcement/receive-arm operation rather than an applet-specific command.

## Raw completion callback

The raw callback is a separate physical object. It copies incoming bytes into the staging buffer, advances the current cursor, and adds each byte into the running 16-bit checksum.

When the announced endpoint is reached it restores the normal packet receiver and compares the accumulated checksum with the value latched by `0x02`.

On success it:

- advances the accepted-byte accumulator by the announced length;
- prepares response `0x43`.

On checksum mismatch it:

- rewinds the current cursor by the announced block length, returning it to the beginning of the failed block;
- prepares response `0x8E`.

This directly explains why `0x43` is a transport completion response and not another selector-table request.

## `0x0B`: mapped block commit/program

The `0x0B` handler begins at file offset `0x1DB4`. It consumes the currently staged block against the segment map established by `0x16/0x17`.

Direct firmware correlation confirms that the handler:

- indexes the ordered destination-address and extent tables;
- handles transfer boundaries that cross from one registered segment into the next;
- follows the Small-ROM programming path for the mapped destination;
- clears the accepted-byte accumulator after the block is committed;
- restores the current staging cursor to the staging base for the next block;
- prepares response `0x47`.

Combined with the independently closed `0x16/0x17` mapper, this establishes `0x0B` as the commit/program stage for the staged block in the active Small-ROM destination context.

## `0x07`: deferred transaction finalization

The Small-ROM `0x07` handler is notably small. At file offset `0x1F8C` it sets a deferred-finalization flag and prepares response `0x48`.

The transport layer later observes that flag, waits for its required condition, clears it, and invokes a further internal helper. The exact source/vendor identity and complete policy of that final helper are not yet independently demonstrated, so they remain `NO_RESUELTO / EVIDENCIA_INSUFICIENTE`.

This is also an important negative result: the much larger principal-OS `0x07` implementation must not be transplanted mechanically into the Small ROM. The two contexts expose the same host-visible finalization contract while using different internal implementations.

## Recovered state machine

The directly verified Small-ROM engine can therefore be represented as:

```text
DESTINATION_SETUP:
    0x16 clear segment map
    0x17 register/erase segment(s)

COMMON_BLOCK_ENGINE per block:
    0x02 announce(len,sum16)
      -> response 0x42
      -> raw receive/copy/sum16
      -> checksum OK: response 0x43
         checksum bad: rewind + response 0x8E
      -> 0x0B mapped program/commit
      -> response 0x47
      -> staging re-armed

FINALIZE:
    0x07 set deferred-finalize state
      -> response 0x48
      -> transport-side completion path
```

The key architectural conclusion is that destination policy and transfer framing are separate. The same Manager block envelope can feed different backends as long as the active firmware context provides the appropriate destination setup and commit policy.

## Verification status

A private reproducible regression against the canonical Small ROM passes **38/38 assertions**. It verifies the image hash, dispatcher selector-to-handler mapping, the `0x02` receive-state setup, raw-copy/checksum success and rewind paths, `0x0B` segment-map/programming handoff and staging reset, and the `0x07` deferred-finalization flag/response.

An earlier 23/38 development run used displaced test-fixture offsets; it was corrected before closure and is not counted as evidence.

Status:

- Small-ROM physical dispatcher mapping: **CONFIRMED**.
- `0x02` block receive/length/checksum contract: **CONFIRMED**.
- raw-copy and `sum16` validation, `0x43` success and `0x8E` rewind/error path: **CONFIRMED**.
- `0x0B` staged mapped block commit/program role: **CONFIRMED** by direct firmware plus the independently verified segment mapper and source-first updater flow.
- `0x07` host-visible finalize contract and deferred flag: **CONFIRMED**.
- exact semantic identity/name of the post-`0x07` internal helper: **UNKNOWN / INSUFFICIENT EVIDENCE**.

No firmware image, large disassembly, or Ghidra artifact is published here. No A-line syscall is implied; the demonstrated A-line frontier remains A470.