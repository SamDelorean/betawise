# OS3K Small ROM selector `0x09`: baud-rate negotiation

This note documents a Manager/update-protocol handler in the AlphaSmart 3000 and NEO Small ROMs. It is **not** an A-line syscall and does not extend the demonstrated A-line ABI frontier.

## Source-first identity

Two independent public implementations agree on the request identity:

- AlphaSync defines request `0x09` as `SET_BAUDRATE` with a 32-bit baud-rate argument.
- neotools independently defines the same request and argument shape.

Both sources identify response `0x4A` as the baud-rate response. They leave response `0x49` unnamed. This note therefore does not invent a vendor name for `0x49`.

## Direct firmware verification

The historical AlphaSmart 3000 Small ROM from AlphaSmart Manager 2.3 and the NEO Small ROM from NEO Manager 3.9.3 use the same reverse-indexed Manager dispatcher namespace. Selector `0x09` resolves to file offset `0x2976` in the AlphaSmart 3000 image and `0x1FC2` in the NEO image.

The two handler objects are each `0x108` bytes. Across all 264 bytes, only five bytes differ, all in the serial/timer programming portion of the handler.

At entry, both handlers compare the 32-bit request value against exactly these five rates:

```text
9600
19200
38400
57600
115200
```

No sixth comparison of the same `CMPI.L #rate,(A6)` form occurs in either object.

For an accepted rate, the shared protocol shape is:

```text
request 0x09(baud32)
    -> response 0x49
    -> serial/timer reconfiguration
    -> wait for serial status
    -> response 0x4A
```

The reconfiguration writes the same serial/timer MMIO register set in both generations (`F600`, `F602`, `F604`) and polls status at `F60B`. The rate-dependent divisor constants are where the small generational byte differences occur. The invalid-rate path contains response `0x92`.

The public source names support `0x09 = SET_BAUDRATE` and `0x4A = baud-rate response`. The exact protocol meaning of preliminary response `0x49` and the vendor name, if any, for error `0x92` remain intentionally unresolved.

## Neighboring unresolved selectors

The same Small-ROM dispatcher also contains selectors `0x0A` and `0x19`, but they are not promoted to named contracts here:

- `0x0A` remains unknown in the public protocol sources. Its Small-ROM handler is only `0x12` bytes in both generations and does not itself contain literal response `0x90`, despite empirical reports of that response. Following the helper/state path is still required.
- `0x19` remains unknown in public sources. Its `0x22`-byte handler is mechanically stable across both generations: it tests a word argument, sets or clears a byte flag, and emits response `0x57`. The meaning of that flag is not demonstrated. A historical suggestion that the request might relate to AlphaHub devices is therefore treated only as a provisional hypothesis.

## Verification boundary

A private reproducible two-image regression passes **45/45 assertions**. It verifies the two Small-ROM hashes, selector object sizes, five accepted baud rates, exact comparison set, `0x49`/`0x4A`/`0x92` protocol envelope, serial-register programming pattern, status wait, and the mechanically stable portions of selectors `0x0A` and `0x19`.

Status:

- selector `0x09` request identity and argument: **CONFIRMED**;
- accepted baud-rate set: **CONFIRMED**;
- two-generation handler homology: **CONFIRMED**;
- response `0x4A` association with baud-rate negotiation: **CONFIRMED**;
- exact semantic name of response `0x49`: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x0A`: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x19` boolean mechanics: **CONFIRMED**, nominal purpose **UNRESOLVED / INSUFFICIENT EVIDENCE**.

No ROM bytes or extended disassembly are published here.