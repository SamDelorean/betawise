# SYS_A03C — selection backing-store character save

Status: **mechanically closed / confidence A**.

`SYS_A03C` is syscall index 15 (`0xA03C`). The portable contract supported by the firmware is:

```c
void SYS_A03C(uint8_t ch);
```

Only the low byte of the caller argument is consumed. The routine writes that byte to the selection/display backing-store cell addressed by the current logical cursor position, using 1-based cursor globals. It does **not** render the character, advance the cursor, refresh the LCD, or issue display-controller I/O.

The implementation is generation-dependent only in backing-store geometry: AlphaSmart 3000 uses a 40-byte row stride, while the 2005 and 2013 NEO implementations use a 132-byte row stride. All three implement the same observable operation. No bounds checks are present, so the logical cursor must already be valid and 1-based.

Historical AlphaSmart source provides a strong genealogy anchor in `LCDSaveScreenData(UInt8 iubChar)`, whose body stores the input byte at the current cursor location in the LCD backing array. The modern vendor-visible symbol has not been recovered, so the SDK keeps the neutral name `SYS_A03C` rather than inventing a vendor API name.

The older BetaWise comment `putch to shadow` was directionally close but potentially misleading: this is a backing-store save primitive, not a visual `PutChar` equivalent.

## Verification

The index-15 vector was revalidated independently in the canonical AlphaSmart 3000 2005, NEO 2005, and NEO 2013 firmware images. In every generation it resolves to the same compact store operation; the immediately following A040/A044 vector entries remain null where applicable.

A source-first static regression was executed and passed **61/61** checks, covering canonical-image identity, vector resolution, handler boundaries, backing-store address arithmetic, argument consumption, absence of display I/O/helper calls, historical source correlation, and official AlphaWordPlus import veneers. Structural executable-xref scans of AlphaWordPlus 2005 and NEO 2012 found the A03C veneer but no callsites; this is recorded as a negative caller search, not as evidence against the syscall.

Dynamic hardware probing has not been executed and remains optional follow-up evidence rather than a prerequisite for the mechanically observable contract above.
