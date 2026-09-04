# A2F0 mechanical closure

Status: **mechanical confidence A**. Original vendor name, argument meanings, internal field names, and status enum names remain unknown.

## Neutral contract

```c
uint32_t SYS_A2F0(uint32_t arg1_slot,
                  void *state,
                  uint32_t arg3_slot);
```

Official callers construct exactly three physical 32-bit stack slots. The handler consumes only `arg1_slot[15:0]`, treats `state` as a mutable pointer, and consumes only `arg3_slot[7:0]`. The declaration deliberately retains full slot widths rather than assuming historical C promotion rules.

`D0.L` is contractual. The explicit success path sets it to zero; failure paths propagate raw longword status values. No public enum or semantic status names are inferred.

## Handler evidence

- AS3000 2005: runtime `0x004D9EA8`, file `+0x019EA8`, length `0x480`, raw SHA-256 `54ac011bd233ce2366333b50a7e143796a8ace2273441020393210f6bdabda9d`.
- NEO 2005: runtime `0x005DC5DC`, file `+0x01C5DC`, length `0x48C`, raw SHA-256 `281b0e8768b2e08862f5354b4fc03bb6773a43c05ee493ee67df57b501f2ed16`.
- NEO 2013: runtime `0x0042ECDC`, file `+0x01ECDC`, length `0x4AA`, raw SHA-256 `0ff682ff855c7fd9e2c9be804ace5eeb1e982be420a0d74d6612ab2c05f47ac4`.

A reproducible 68000 CFG pass reports no unknown opcodes. All reachable `BRA/Bcc/DBcc` destinations stay within the handler and land on instruction boundaries; each compared body terminates at one reachable final `RTS`. Code immediately afterward begins an independent prologue and is not attributed to A2F0.

## Physical ABI and return

After register save/local allocation, the handler loads the middle physical slot as its mutable state pointer. A later word read that superficially appears at a different stack displacement occurs after a temporary `PEA`; normalized to the base frame it is the low word of the first slot. A byte test at the base-frame displacement for the third slot consumes only its low byte.

Concrete ROM callers construct the equivalent of:

1. zero-extended 16-bit value;
2. pointer to mutable local/state;
3. literal `1` in the observed direct caller.

They remove `0x0C` bytes and immediately execute `TST.L D0`. NEO wrappers reconstruct the same three-slot shape and propagate D0 to their own callers.

The compared epilogues converge on either explicit `MOVEQ #0,D0` or `MOVE.L D7,D0`, where D7 carries helper status. A literal `0x01000001` also occurs on one failure route. The public type therefore preserves the raw 32-bit status rather than inventing an enum or signed-error convention.

## State and helper graph

The writable state object is observed through at least offset `+0x2E`, so callers must provide storage covering at least `0x30` bytes. Common accesses include a word at `+0x00`; longwords at `+0x02`, `+0x06`, `+0x0A`, `+0x0E`, `+0x12`; and words at `+0x1A`, `+0x22`, `+0x26`, `+0x2A`, `+0x2C`, and `+0x2E`. NEO 2005/2013 additionally store a helper result at longword `+0x16`.

The helper graph obtains a base pointer and an end/length value, initializes several state offsets to `-1`, scans a byte-oriented region, invokes region-adjustment helpers, and finally commits/validates the state. These are neutral mechanical descriptions only; no vendor structure or helper names are assigned.

## Generational comparison

All three generations preserve the external three-slot ABI and raw D0.L status behavior.

AS3000 and NEO 2005 are highly homologous. NEO 2005 adds the state `+0x16` initialization. NEO 2013 keeps that extension, changes internal address-register allocation, adds preparation calls, and contains a real parser-marker difference: the first compared marker remains `0xA8`, but the second changes from `0xA9` in AS3000/NEO 2005 to `0x9D` in NEO 2013. This difference is retained rather than normalized.

The parser also compares bytes such as `0x26`, `0x23`, `0x3B`, ASCII `0` through `9`, `0x3C`, quote characters, and `0x3E`. Their higher-level format meaning is intentionally not inferred.

## Official caller corpus

The SmartApplet call idiom is resolved through the A-line table using `MOVEA.L #disp32,A0` followed by `JSR (-2,PC,A0.L)`.

An exhaustive scan covered **41 official applets**: 16 SmartApplets3000-era files and 25 NEO-era files. Thirty contain the A-line table and were rematerialized, SHA-256 checked against the canonical manifest, and scanned for executable A2F0 targets. Eleven applets without an A-line table are documented structural negatives.

Exactly two applets contain A2F0 calls:

- AlphaWordPlus 2005, SHA-256 `22fc6a9b7ab96b332965074d81e88a61574c82bd118cd0a5122ff17eda348515`: six calls at file offsets `+0x0C42`, `+0x13CE`, `+0x9810`, `+0xD0E0`, `+0xD248`, `+0xD306`.
- AlphaWordPlus NEO 2012, SHA-256 `a8ae516734a0e0b2e19d51ffb7dff3893074ca733fa40207cbff7ead1433f29e`: six calls at `+0x1048`, `+0x189E`, `+0xA284`, `+0xDB5A`, `+0xDCC4`, `+0xDD82`.

No A2F0 caller occurs in the other 39 official applets. Independent sites consume or propagate D0.L, corroborating the ROM-side return analysis.

## Adversarial review

- The apparent `SP+0x38` / `SP+0x3A` overlap was refuted by normalizing the temporary `PEA` stack adjustment; there is no tagged-pointer or overlapping-argument ABI.
- D0 is not incidental residue: ROM and SmartApplet callers consume it as a longword and the handler defines it explicitly on all terminal routes.
- The second aligned `RTS` before the next A-line vector belongs to a separate private function; CFG closure prevents accidental over-attribution.
- The NEO 2013 `0x9D` marker is a real generational difference and is not rewritten as `0xA9`.
- No semantic names are assigned to arguments, state fields, markers, helpers, or status values.

## Regression status

A private static regression was **executed and passed**. It revalidated all three canonical ROM SHA-256 values, all three handler lengths/hashes and final `RTS` bytes, key ABI access patterns, the `0xA9` versus `0x9D` generational marker, and the complete 41/41 official caller corpus.

The first regression pass caught and corrected a one-character transcription error in the private workpaper SHA-256 for `text2speechupdate.os3kapp`; the binary and canonical manifest agreed, the workpaper was fixed, and the complete regression then passed. This correction is retained in the private chronological record.

Dynamic/emulator regression is not claimed as executed. Private ROM bytes, extensive disassembly, helper listings, and exhaustive file manifests remain in Drive.

## 2026-09-04 source-first re-audit

The closure was reclassified under SOURCE-FIRST without changing its neutral ABI. The source-first anchor is the exhaustive official SmartApplet corpus together with direct ROM callers: official AlphaWordPlus 2005 and NEO 2012 provide twelve executable A2F0 invocations in total, while the other 39 applets are documented negatives. These callers independently demonstrate three physical 32-bit slots and consume or propagate the returned full D0 longword. Direct ROM callers likewise clean exactly `0x0C` bytes and execute `TST.L D0` immediately.

The firmware remains authoritative for slot consumption: low16 of slot 1, mutable pointer slot 2, low8 of slot 3. It also proves explicit full-long success/failure construction, including propagated helper statuses and literal `0x01000001`. Thus `uint32_t SYS_A2F0(uint32_t arg1_slot, void *state, uint32_t arg3_slot)` remains the maximum justified public contract; no vendor name, argument semantics, marker semantics, or status enum is recovered.

Static source-first correlation regression: **18/18 PASS** (3 canonical ROM identities, 3 exact handler fingerprint/length checks, 3 terminal CFG checks, 3 physical-slot access checks, 2 official applet generation caller sets, 2 exact three-slot cleanup checks, 2 full-long D0-consumption/propagation checks). Dynamic/emulator regression remains **specified / not executed**.
