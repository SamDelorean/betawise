# OS3K Small ROM selector `0x09`: baud-rate negotiation

This note documents a Manager/update-protocol handler in the AlphaSmart 3000 and NEO Small ROMs. It is **not** an A-line syscall and does not extend the demonstrated A-line ABI frontier.

## Source-first identity

AlphaSync defines request `0x09` as `SET_BAUDRATE` with a 32-bit baud-rate argument and identifies response `0x4A` as the baud-rate response. neotools carries the same request/response names and packet comments, but its own README states that its device driver was ported from AlphaSync. It is therefore useful corroboration of implementation lineage, **not an independent source** for the protocol identity.

This distinction matters for neighboring unknown selectors as well: wording in neotools that is textually inherited from AlphaSync must not be counted as a second evidentiary vote.

Both codebases leave response `0x49` unnamed. This note therefore does not invent a vendor name for `0x49`.

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

AlphaSync supplies the historical name `SET_BAUDRATE`; direct firmware then independently confirms the argument width, accepted rate set, serial-register behavior, and `0x4A` success envelope. The exact protocol meaning of preliminary response `0x49` and the vendor name, if any, for error `0x92` remain intentionally unresolved.

## Neighboring unresolved selectors

The same Small-ROM dispatcher also contains selectors `0x0A` and `0x19`, but they are not promoted to named contracts here:

- `0x0A` remains unknown in AlphaSync; neotools repeats the same unknown/empirical-`0x90` comment through the AlphaSync-derived driver lineage, so that repetition is not independent evidence. Its Small-ROM handler is only `0x12` bytes in both generations and does not itself contain literal response `0x90`, despite the historical empirical report. Following the helper/state path is still required.
- `0x19` remains unknown. Its `0x22`-byte handler is mechanically stable across both generations: it tests a word argument, sets or clears a byte flag, and emits response `0x57`. The often-repeated suggestion that the request "may be specific to AlphaHub devices" originates as an explicitly speculative AlphaSync comment and is copied into neotools. No independent AlphaHub contract/caller/source has been demonstrated. The AlphaHub association is therefore **DESCONOCIDO / unsupported as an identity claim**, not a corroborated provisional contract.

## Verification boundary

A private reproducible two-image regression passes **45/45 assertions**. It verifies the two Small-ROM hashes, selector object sizes, five accepted baud rates, exact comparison set, `0x49`/`0x4A`/`0x92` protocol envelope, serial-register programming pattern, status wait, and the mechanically stable portions of selectors `0x0A` and `0x19`.

Status:

- selector `0x09` request identity and argument: **CONFIRMED** (historical AlphaSync naming + independent firmware mechanics);
- accepted baud-rate set: **CONFIRMED**;
- two-generation handler homology: **CONFIRMED**;
- response `0x4A` association with baud-rate negotiation: **CONFIRMED**;
- exact semantic name of response `0x49`: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x0A`: **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- selector `0x19` boolean mechanics: **CONFIRMED**, nominal purpose **UNRESOLVED / INSUFFICIENT EVIDENCE**;
- AlphaHub identity for `0x19`: **DESCONOCIDO; source-lineage speculation only**.

No ROM bytes or extended disassembly are published here.
