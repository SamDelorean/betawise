# SYS_A468 / index 282 — ABI trace

Status: **mechanically reconstructed (A), source-level identity blocked**.

## Availability

| Platform | Slot disposition |
|---|---|
| AlphaSmart 3000 (2005) | raw slot value `0x050719E0`; outside the demonstrated ROM/runtime window, so no callable contract is claimed |
| NEO (2005) | raw slot value `0x050719E0`; outside the demonstrated ROM/runtime window, so no callable contract is claimed |
| NEO (2013) | slot resolves to runtime `0x00434B14` |

A468 is therefore callable only in the canonical NEO 2013 image. The official SmartApplet corpus nevertheless exposes an A468 stub in all 16 EXTENDED applets; no executable caller of that stub was recovered.

## Confirmed mechanical ABI

The NEO 2013 handler consumes four 32-bit physical stack slots:

1. slot 1: only the low byte is consumed;
2. slot 2: full 32 bits are stored;
3. slot 3: full 32 bits are stored;
4. slot 4: full 32 bits are stored.

On the insertion path it writes a fixed 14-byte record into a circular/ring-like RAM table with capacity 200. The recovered layout is:

- record `+0`: current ring index byte;
- record `+1`: slot-1 low byte;
- record `+2..+5`: slot 2;
- record `+6..+9`: slot 3;
- record `+10..+13`: slot 4.

The write index wraps at 200. A separate counter is incremented after insertion. The handler performs the mutation with interrupts masked to level 7 and restores the previous status register afterward.

No vendor/source-level meaning is assigned to the record, its fields, or the associated globals.

## Status byte

Only `D0.low8` has a demonstrated return contract:

- `1` on the first gate-failure path;
- `2` when the recovered capacity predicate reports the bounded structure full;
- `0` after a successful insertion path.

The success path clears only the low byte after a post-insertion helper, so this evidence does **not** justify claiming that the complete 32-bit `D0` value is zero. Recovered firmware callers that consume the result do so as a byte; several others ignore it.

## Callers and xrefs

The canonical 41-SmartApplet corpus is negative for executable A468 callers: all 16 EXTENDED hashes/stubs were revalidated and searched using the established indexed-PC call pattern plus direct A-line and relative/indexed branch-call alternatives; 14 LEGACY applets end before this range and 11 have no A-line table.

NEO 2013 firmware contains exactly seven direct executable xrefs to the handler: five absolute-long calls and two word-relative calls. Recovered caller setups include both slot-1 values `0` and `3`, pointer-valued slots, and a four-way caller whose branches all use `(0, pointer, 0, 0)` with four different constant pointers. This is sufficient to validate the four-slot ABI without assigning source-level parameter names.

## Publication boundary

The public symbol remains neutral: `SYS_A468`. No vendor function name, enum/status names, record semantic name, or field names are asserted. ROM bytes and extended disassembly remain only in private workpapers.

Private regression revalidated all three canonical ROM hashes, handler/helper boundaries and hashes, ring mechanics, firmware xrefs, caller setup, and the official applet corpus. Its final expanded run passed 153/153 checks after an earlier adversarial run exposed and corrected transcription errors.
