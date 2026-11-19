# System 3 / OS3K ABI reconstruction index

This is the **canonical navigation and traceability index** for reconstructed
System 3 / OS3K interfaces in this repository. It answers two questions that
must remain separate:

1. **What is the current contract that a developer may rely on?**
2. **Where is the evidence and research history that produced that contract?**

The older `file-api-reconstruction.md` is intentionally retained as a
**chronological research log** and contains provisional statements later
superseded by stronger evidence. When a closure/current-reference document
exists, use it for the current SDK contract.

## Evidence and naming policy

- **Mechanical confidence A**: argument width/order, return behavior, state
  mutation or control flow is established directly from firmware or equivalent
  primary evidence.
- Historical/name confidence is tracked separately from mechanical confidence.
- A recovered historical AS3000 name is reused only when later System 3 clearly
  implements the same conceptual API member; ABI evolution is documented.
- When behavior is closed but the original modern name is not, the interface
  remains `SYS_Axxx` rather than receiving a descriptive guess.
- A regression specification is not evidence that the test has been run.

Each closed block documents purpose, prototype, arguments/returns, state effects,
errors/sentinels, cross-ROM differences, caller/source evidence, safety,
validation status and concrete audit references.

## Current reconstructed blocks

The table below covers the canonical index through A28C. Continue with
[`abi-reconstruction-index-a290-a470.md`](abi-reconstruction-index-a290-a470.md)
for A290–A470. That continuation uses the same evidence policy; inclusion there
does not promote `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entries to public SDK
contracts.

| Trap/range | Current identity | Status | Canonical documentation |
| --- | --- | --- | --- |
| A000 | raw `_OS3K_ClearScreen(void)` / public `ClearScreen(void)` | mechanical A; clears display, hides OS cursor and restores OS cursor to 1,1; BetaWise wrapper additionally resets its private scroll/start-line state | [`clear-screen-closure.md`](clear-screen-closure.md), `os3k/os3k.h` |
| A004 | `_OS3K_SetCursor` | mechanical confidence A for the raw System 3 ABI. The public BetaWise `SetCursor()` wrapper remains a separate higher-level layer | [`setcursor-closure.md`](setcursor-closure.md) (current) |
| A008 | `GetCursorPos` | mechanical confidence A for the System 3 ABI | [`getcursorpos-closure.md`](getcursorpos-closure.md) (current) |
| A00C | `PutStringCentered` | mechanical confidence A for the System 3 ABI | [`putstringcentered-closure.md`](putstringcentered-closure.md) (current) |
| A010 | `PutChar` | mechanical confidence A for the System 3 ABI | [`putchar-closure.md`](putchar-closure.md) (current) |
| A014 | `PutStringRaw` | mechanical confidence A for the System 3 ABI | [`putstringraw-closure.md`](putstringraw-closure.md) (current) |
| A018 | `SetCursorMode` | see closure | [`setcursormode-closure.md`](setcursormode-closure.md) (current) |
| A01C | `GetCursorMode` | see closure | [`getcursormode-closure.md`](getcursormode-closure.md) (current) |
| A020 | `ClearRowCols` | mechanically confirmed (A) against the canonical AS3000 2005, NEO 2005 and NEO 2013 System 3 ROMs | [`clearrowcols-closure.md`](clearrowcols-closure.md) (current) |
| A024 | `ClearRows` | mechanically closed / source-first revalidated | [`clearrows-closure.md`](clearrows-closure.md) (current) |
| A028 | `SYS_A028` | mechanically closed (A confidence) for the November 2005 AlphaSmart 3000 and NEO System 3 images and the July 2013 NEO System 3 image | [`sys-a028-screen-snapshot-closure.md`](sys-a028-screen-snapshot-closure.md) (current) |
| A02C | `SYS_A02C` | mechanically closed (A confidence) for the November 2005 AlphaSmart 3000 and NEO System 3 images and the July 2013 NEO System 3 image | [`sys-a02c-screen-restore-closure.md`](sys-a02c-screen-restore-closure.md) (current) |
| A030 | `PutString` | see closure | [`putstring-closure.md`](putstring-closure.md) (current) |
| A034 | `SYS_A034` | mechanical confidence A. Historical/name confidence is tracked separately | [`sys-a034-selection-data-clear-closure.md`](sys-a034-selection-data-clear-closure.md) (current) |
| A038 | `SYS_A038` | mechanical confidence A, with one intentionally unresolved ABI detail documented below | [`sys-a038-selection-refresh-closure.md`](sys-a038-selection-refresh-closure.md) (current) |
| A03C | `SYS_A03C` | mechanically closed / confidence A | [`sys-a03c-selection-backing-store-closure.md`](sys-a03c-selection-backing-store-closure.md) (current) |
| A040 | null vector / no callable contract | mechanically closed / source-first revalidated / null vector confirmed. No callable prototype is published. The original vendor purpose or l | [`sys-a040-null-vector.md`](sys-a040-null-vector.md) (current) |
| A044 | null vector / no callable contract | CONFIRMED for the canonical AS3000 2005, NEO 2005, and NEO 2013 firmware images | [`sys-a044-null-vector.md`](sys-a044-null-vector.md) (current) |
| A048 | `DrawBitmap` | MECÁNICA_CERRADA A / REVALIDADO_SOURCE_FIRST | [`drawbitmap-closure.md`](drawbitmap-closure.md) (current) |
| A04C | `RasterOp` | mechanically closed / source-first revalidated | [`rasterop-closure.md`](rasterop-closure.md) (current) |
| A050 | null vector / no callable contract | `CLOSED_MECHANICAL / REVALIDATED_SOURCE_FIRST` | [`sys-a050-a068-null-vectors.md`](sys-a050-a068-null-vectors.md) (current) |
| A068 | null vector / no callable contract | `CLOSED_MECHANICAL / REVALIDATED_SOURCE_FIRST` | [`sys-a050-a068-null-vectors.md`](sys-a050-a068-null-vectors.md) (current) |
| A06C | `SYS_A06C` | `CLOSED_MECHANICAL / REVALIDATED_SOURCE_FIRST` | [`sys-a06c-strlen-equivalent-closure.md`](sys-a06c-strlen-equivalent-closure.md) (current) |
| A070 | `SYS_A070` | source-first closure, confidence A | [`sys-a070-char-search-closure.md`](sys-a070-char-search-closure.md) (current) |
| A074 | `SYS_A074` | mechanically closed from primary firmware evidence | [`sys-a074-inclusive-byte-range-search-closure.md`](sys-a074-inclusive-byte-range-search-closure.md) (current) |
| A078 | `SYS_A078` | mechanically closed from primary firmware evidence and revalidated source-first | [`sys-a078-bounded-pattern-search-closure.md`](sys-a078-bounded-pattern-search-closure.md) (current) |
| A07C | `SYS_A07C` | mechanically closed from primary firmware evidence and revalidated source-first | [`sys-a07c-radix-conversion-closure.md`](sys-a07c-radix-conversion-closure.md) (current) |
| A080 | `ProgressBar` | mechanically closed from primary firmware evidence and revalidated source-first | [`progressbar-closure.md`](progressbar-closure.md) (current) |
| A084 | `TextBox` | mechanically closed from primary firmware evidence and revalidated source-first | [`textbox-closure.md`](textbox-closure.md) (current) |
| A088 | `WaitForKey` | mechanically closed / source-first revalidated | [`waitforkey-closure.md`](waitforkey-closure.md) (current) |
| A08C | `StringCompare` | source-first closure, firmware revalidated on the three canonical OS3K ROM generations | [`stringcompare-closure.md`](stringcompare-closure.md) (current) |
| A090 | `DisplayMessage` | source-first closure, firmware revalidated on the three canonical OS3K ROM generations | [`displaymessage-closure.md`](displaymessage-closure.md) (current) |
| A094 | `GetKey` | source-first closure, revalidated against the three canonical OS3K ROM generations | [`getkey-closure.md`](getkey-closure.md) (current) |
| A098 | `DrainKeyBuffer` | source-first closure, revalidated against the three canonical OS3K ROM generations | [`drainkeybuffer-closure.md`](drainkeybuffer-closure.md) (current) |
| A09C | `IsKeyReady` | source-first closure, revalidated against the three canonical OS3K ROM generations | [`iskeyready-closure.md`](iskeyready-closure.md) (current) |
| A0A0 | `GetModifierKeys` | see closure | [`getmodifierkeys-closure.md`](getmodifierkeys-closure.md) (current) |
| A0A4 | `ScanKeyboard` | see closure | [`scankeyboard-closure.md`](scankeyboard-closure.md) (current) |
| A0A8 | `QueueKey` | see closure | [`queuekey-closure.md`](queuekey-closure.md) (current) |
| A0AC | `SetModifierKeys` | see closure | [`setmodifierkeys-closure.md`](setmodifierkeys-closure.md) (current) |
| A0B0 | `IsKeyDownNow` | see closure | [`iskeydownnow-closure.md`](iskeydownnow-closure.md) (current) |
| A0B4 | `SYS_A0B4` | see closure | [`sys-a0b4-physical-matrix-bit-test-closure.md`](sys-a0b4-physical-matrix-bit-test-closure.md) (current) |
| A0B8 | `SYS_A0B8` | CLOSED (source-first) | [`sys-a0b8-auto-repeat-state-closure.md`](sys-a0b8-auto-repeat-state-closure.md) (current) |
| A0BC | `SYS_A0BC` | CLOSED (source-first) | [`sys-a0bc-sticky-keys-state-closure.md`](sys-a0bc-sticky-keys-state-closure.md) (current) |
| A0C0 | `SYS_A0C0` | CLOSED (source-first) | [`sys-a0c0-sticky-keys-state-setter-closure.md`](sys-a0c0-sticky-keys-state-setter-closure.md) (current) |
| A0C4 | `SYS_A0C4` | CLOSED (source-first) | [`sys-a0c4-slow-keys-state-closure.md`](sys-a0c4-slow-keys-state-closure.md) (current) |
| A0C8 | `SYS_A0C8` | mechanically closed / source-first revalidated | [`sys-a0c8-slow-key-delay-closure.md`](sys-a0c8-slow-key-delay-closure.md) (current) |
| A0CC | `SYS_A0CC` | mechanically closed / source-first revalidated | [`sys-a0cc-show-marks-flag-closure.md`](sys-a0cc-show-marks-flag-closure.md) (current) |
| A0D0 | `SYS_A0D0` | mechanically closed / source-first revalidated | [`sys-a0d0-show-marks-flag-setter-closure.md`](sys-a0d0-show-marks-flag-setter-closure.md) (current) |
| A0D4 | `SleepCentiseconds` | mechanically closed (confidence A) under the source-first ABI audit | [`sleepcentiseconds-closure.md`](sleepcentiseconds-closure.md) (current) |
| A0D8 | `SleepCentimilliseconds` | mechanically closed (confidence A) under the source-first ABI audit | [`sleepcentimilliseconds-closure.md`](sleepcentimilliseconds-closure.md) (current) |
| A0DC | `GetUptimeSeconds` | mechanically closed (source-first audit, 2026-09-02) | [`getuptimeseconds-closure.md`](getuptimeseconds-closure.md) (current) |
| A0E0 | `GetUptimeCentiseconds` | mechanically closed (source-first audit, 2026-09-02) | [`getuptimecentiseconds-closure.md`](getuptimecentiseconds-closure.md) (current) |
| A0E4 | `GetUptimeMilliseconds` | mechanically closed (source-first audit, 2026-09-02) | [`getuptimemilliseconds-closure.md`](getuptimemilliseconds-closure.md) (current) |
| A0E8 | `SYS_A0E8` | mechanically closed (A). Public name remains `SYS_A0E8`; no vendor-era symbol has been recovered for this 2005-era interface | [`sys-a0e8-timer-arm-closure.md`](sys-a0e8-timer-arm-closure.md) (current) |
| A0EC | `SYS_A0EC` | mechanically closed (A). Public name remains `SYS_A0EC`; no vendor-era modern symbol has been recovered | [`sys-a0ec-timer-cancel-closure.md`](sys-a0ec-timer-cancel-closure.md) (current) |
| A0F0–A110 | Dialog API (`DialogInit` through `DialogGetItemId`) | mechanically closed for normal public behavior; internal event codes remain unnamed | [`dialog-api-closure.md`](dialog-api-closure.md) |
| A114 | `SYS_A114` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a114-dialog-item-text-closure.md`](sys-a114-dialog-item-text-closure.md) (current) |
| A118 | `SYS_A118` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a118-byte-state-getter-closure.md`](sys-a118-byte-state-getter-closure.md) (current) |
| A11C | `SYS_A11C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a11c-byte-state-setter-closure.md`](sys-a11c-byte-state-setter-closure.md) (current) |
| A120 | `SYS_A120` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a120-battery-measurement-refresh-closure.md`](sys-a120-battery-measurement-refresh-closure.md) (current) |
| A124 | `SYS_A124` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a124-battery-profile-getter-closure.md`](sys-a124-battery-profile-getter-closure.md) (current) |
| A128 | `SYS_A128` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a128-battery-capacity-percentage-closure.md`](sys-a128-battery-capacity-percentage-closure.md) (current) |
| A12C | `SYS_A12C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a12c-battery-voltage-millivolts-closure.md`](sys-a12c-battery-voltage-millivolts-closure.md) (current) |
| A130 | `SYS_A130` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a130-board-revision-getter-closure.md`](sys-a130-board-revision-getter-closure.md) (current) |
| A134 | `SYS_A134` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a134-battery-diagnostic-mux-closure.md`](sys-a134-battery-diagnostic-mux-closure.md) (current) |
| A138 | `ShowBatteryPercentage(uint8_t time_seconds)` | ABI/parameter semantics closed | [`battery-api-closure.md`](battery-api-closure.md) |
| A13C | `SYS_A13C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a13c-uart-serial-state-init-closure.md`](sys-a13c-uart-serial-state-init-closure.md) (current) |
| A140 | `SYS_A140` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a140-serial-configuration-closure.md`](sys-a140-serial-configuration-closure.md) (current) |
| A144 | `SYS_A144` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a144-uart-baud-closure.md`](sys-a144-uart-baud-closure.md) (current) |
| A148 | `SYS_A148` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a148-uart-status-closure.md`](sys-a148-uart-status-closure.md) (current) |
| A14C | `SYS_A14C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a14c-uart-rx-callback-closure.md`](sys-a14c-uart-rx-callback-closure.md) (current) |
| A150 | `SYS_A150` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a150-uart-receiver-disable-closure.md`](sys-a150-uart-receiver-disable-closure.md) (current) |
| A154 | `SYS_A154` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a154-uart-receiver-enable-closure.md`](sys-a154-uart-receiver-enable-closure.md) (current) |
| A158 | `SYS_A158` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a158-uart-receive-buffer-closure.md`](sys-a158-uart-receive-buffer-closure.md) (current) |
| A15C | `SYS_A15C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a15c-uart-transmit-buffer-closure.md`](sys-a15c-uart-transmit-buffer-closure.md) (current) |
| A160 | `SYS_A160` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a160-byte-case-fold-closure.md`](sys-a160-byte-case-fold-closure.md) (current) |
| A164 | `_OS3K_TranslateKeyToChar` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`translatekeytochar-source-first-closure.md`](translatekeytochar-source-first-closure.md) (current) |
| A168 | `SYS_A168` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a168-key-layout-remap-closure.md`](sys-a168-key-layout-remap-closure.md) (current) |
| A16C | `SYS_A16C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a16c-byte-membership-closure.md`](sys-a16c-byte-membership-closure.md) (current) |
| A170 | `SYS_A170` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a170-keyboard-layout-service-closure.md`](sys-a170-keyboard-layout-service-closure.md) (current) |
| A174 | `SYS_A174` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a174-byte-state-getter-closure.md`](sys-a174-byte-state-getter-closure.md) (current) |
| A178 | `SYS_A178` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a178-byte-state-setter-closure.md`](sys-a178-byte-state-setter-closure.md) (current) |
| A17C | `SYS_A17C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a17c-byte-state-getter-closure.md`](sys-a17c-byte-state-getter-closure.md) (current) |
| A180 | `SYS_A180` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a180-change-aware-byte-state-setter-closure.md`](sys-a180-change-aware-byte-state-setter-closure.md) (current) |
| A184 | `SYS_A184` | MECHANICALLY CLOSED / SEMANTICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a184-idle-time-getter-closure.md`](sys-a184-idle-time-getter-closure.md) (current) |
| A188 | `SYS_A188` | MECHANICALLY CLOSED / SEMANTICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a188-idle-time-setter-closure.md`](sys-a188-idle-time-setter-closure.md) (current) |
| A18C | `SYS_A18C` | MECHANICALLY CLOSED / SOURCE-FIRST / STATIC REGRESSION EXECUTED | [`sys-a18c-idle-time-restore-closure.md`](sys-a18c-idle-time-restore-closure.md) (current) |
| A190 | `SYS_A190(file_id, offset, origin)` file position seek | mechanically closed; source-first; static regression executed; original name open | [`sys-a190-file-position-closure.md`](sys-a190-file-position-closure.md) (source-first, current) |
| A194 | `SYS_A194(file_id)` file position query | mechanically closed; source-first; static regression executed; original name open | [`sys-a194-file-position-query-closure.md`](sys-a194-file-position-query-closure.md) (source-first, current) |
| A198 | `FileWriteBuffer` | mechanically closed; source-first; static regression executed | [`filewritebuffer-a198-source-first-closure.md`](filewritebuffer-a198-source-first-closure.md) (source-first, current) |
| A19C | `FileReadBuffer` | mechanically closed; source-first; static regression executed | [`filereadbuffer-a19c-source-first-closure.md`](filereadbuffer-a19c-source-first-closure.md) (source-first, current) |
| A1A0 | `FileSmashFile` | closed; destructive | [`filesmashfile-a1a0-source-first-closure.md`](filesmashfile-a1a0-source-first-closure.md) (source-first, current); also: [`file-core-operations.md`](file-core-operations.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1A4 | `FileDeleteChars` | closed | [`filedeletechars-a1a4-source-first-closure.md`](filedeletechars-a1a4-source-first-closure.md) (source-first, current); also: [`filedeletechars-closure.md`](filedeletechars-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1A8 | `FileGetCurrentFile` | mechanical A; SDK consolidated/audited; read-only canonical-token query; regression pending | [`filegetcurrentfile-a1a8-source-first-closure.md`](filegetcurrentfile-a1a8-source-first-closure.md) (source-first, current); also: [`filegetcurrentfile-closure.md`](filegetcurrentfile-closure.md), [`file-core-operations.md`](file-core-operations.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1AC | `SYS_A1AC` global allocator/reclaimable metric | mechanical contract closed; original name open; `+0x10` now identified as `min_size` | [`sys-a1ac-file-space-accounting-source-first-closure.md`](sys-a1ac-file-space-accounting-source-first-closure.md) (source-first, current); also: [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B0 | `SYS_A1B0(file_id)` unused descriptor capacity | mechanical contract closed; original name open | [`sys-a1b0-file-capacity-source-first-closure.md`](sys-a1b0-file-capacity-source-first-closure.md) (source-first, current); also: [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B4 | `SYS_A1B4(token_or_selector)` size/accounting family | mechanical paths closed; selector names open; `0xFE/0xFF` account against `min_size` | [`sys-a1b4-size-accounting-source-first-closure.md`](sys-a1b4-size-accounting-source-first-closure.md) (source-first, current); also: [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1B8 | `SYS_A1B8(size_or_command)` resize/state family | mechanical paths closed; command names partly historical only; `-5` updates `min_size` | [`sys-a1b8-active-file-resize-state-source-first-closure.md`](sys-a1b8-active-file-resize-state-source-first-closure.md) (source-first, current); also: [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1BC | `SYS_A1BC(file_id)` descriptor maximum capacity | mechanical contract closed; original name open | [`sys-a1bc-file-max-size-source-first-closure.md`](sys-a1bc-file-max-size-source-first-closure.md) (source-first, current); also: [`file-space-accounting.md`](file-space-accounting.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1C0 | `FileSetFolder` | closed | [`filesetfolder-a1c0-source-first-closure.md`](filesetfolder-a1c0-source-first-closure.md) (source-first, current); also: [`filesetfolder-closure.md`](filesetfolder-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1C4 | `FileGetFileInfo` | closed; live-mirror lifetime is contractual | [`filegetfileinfo-a1c4-source-first-closure.md`](filegetfileinfo-a1c4-source-first-closure.md) (source-first, current); also: [`file-api-current-reference.md`](file-api-current-reference.md), `applets/FileGetFileInfoProbe/` |
| A1C8/A1CC | `FileOpen` / `FileClose` | closed | [`fileopen-a1c8-source-first-closure.md`](fileopen-a1c8-source-first-closure.md), [`fileclose-a1cc-source-first-closure.md`](fileclose-a1cc-source-first-closure.md) (source-first, current); also: [`fileopen-fileclose-closure.md`](fileopen-fileclose-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1D0/A1D4 | per-file state-bit mutate/query | mechanically closed as `SYS_A1D0/A1D4`; bit names open | [`sys-a1d0-file-state-flags-source-first-closure.md`](sys-a1d0-file-state-flags-source-first-closure.md), [`sys-a1d4-file-state-query-source-first-closure.md`](sys-a1d4-file-state-query-source-first-closure.md) (source-first, current); also: [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1D8 | reset group passwords to factory `write` after master verification | mechanically closed; destructive; original name open | [`sys-a1d8-file-password-reset-source-first-closure.md`](sys-a1d8-file-password-reset-source-first-closure.md) (source-first, current); also: [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1DC | bidirectional per-file password get/set | mechanically closed; original modern name open | [`sys-a1dc-file-password-access-source-first-closure.md`](sys-a1dc-file-password-access-source-first-closure.md) (source-first, current); also: [`file-password-state-closure.md`](file-password-state-closure.md), `os3k/file_password_state.h` |
| A1E0 | `PasswordVerifyFileDialog` | mechanical A; historical/name continuity B+; modern six-argument ABI documented | [`password-verify-file-dialog-source-first-closure.md`](password-verify-file-dialog-source-first-closure.md) (source-first, current); also: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1E4 | `PasswordChangeFileDialog` | mechanical A; historical/name continuity B+; return is token/status, not changed boolean | [`password-change-file-dialog-source-first-closure.md`](password-change-file-dialog-source-first-closure.md) (source-first, current); also: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), [`file-api-current-reference.md`](file-api-current-reference.md) |
| A1E8 | dynamic descriptor/file creation | mechanically closed as `SYS_A1E8`; original name open; establishes descriptor `+0x10 = min_size` | [`sys-a1e8-dynamic-file-create-source-first-closure.md`](sys-a1e8-dynamic-file-create-source-first-closure.md) (source-first, current); also: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1EC | dynamic descriptor/file removal + allocator/table compaction | mechanically closed as `SYS_A1EC`; destructive; original name open | [`sys-a1ec-dynamic-file-remove-source-first-closure.md`](sys-a1ec-dynamic-file-remove-source-first-closure.md) (source-first, current); also: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1F0 | filename get/set (`+0x34`) | mechanically closed as `SYS_A1F0`; original name open | [`sys-a1f0-file-name-source-first-closure.md`](sys-a1f0-file-name-source-first-closure.md) (source-first, current); also: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1F4/A1F8 | local index (`+0x46`) get/set and reverse lookup | mechanically closed as `SYS_A1F4/A1F8`; original names open | [`sys-a1f4-local-index-source-first-closure.md`](sys-a1f4-local-index-source-first-closure.md), [`sys-a1f8-local-index-reverse-lookup-source-first-closure.md`](sys-a1f8-local-index-reverse-lookup-source-first-closure.md) (source-first, current); also: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A1FC | construct/validate 16-bit token | mechanically closed as `SYS_A1FC`; original name open | [`sys-a1fc-token-build-validate-source-first-closure.md`](sys-a1fc-token-build-validate-source-first-closure.md) (source-first, current); also: [`file-identity-dynamic-closure.md`](file-identity-dynamic-closure.md), `os3k/file_dynamic.h` |
| A200 | copy active-file range to clipboard | mechanically closed as `SYS_A200`; original name/tag enum open | [`sys-a200-clipboard-copy-source-first-closure.md`](sys-a200-clipboard-copy-source-first-closure.md) (source-first, current); also: [`clipboard-edit-closure.md`](clipboard-edit-closure.md), `os3k/file_clipboard_edit.h` |
| A204 | cut active-file range to clipboard | mechanically closed as `SYS_A204`; deletes only bytes actually copied | [`sys-a204-clipboard-cut-source-first-closure.md`](sys-a204-clipboard-cut-source-first-closure.md) (source-first, current); also: [`clipboard-edit-closure.md`](clipboard-edit-closure.md), `os3k/file_clipboard_edit.h` |
| A208 | paste clipboard through A198 write modes | mechanically closed as `SYS_A208`; write modes known, read variant 3 is control-code-aware/filtered; original names open | [`sys-a208-clipboard-paste-source-first-closure.md`](sys-a208-clipboard-paste-source-first-closure.md) (source-first, current); also: [`clipboard-edit-closure.md`](clipboard-edit-closure.md), `os3k/file_clipboard_edit.h` |
| A20C | `ClipboardSet` | mechanically closed; raw errors, capacity limiting and shared tag state identified | [`clipboardset-a20c-source-first-closure.md`](clipboardset-a20c-source-first-closure.md) (source-first, current); also: [`clipboard-buffer-closure.md`](clipboard-buffer-closure.md), `os3k/file_clipboard_buffer.h` |
| A210 | `ClipboardGet` | raw path closed and portable in compared ROMs; filtered selector 3 closed mechanically with documented 2005-vs-2013 firmware difference | [`clipboardget-a210-source-first-closure.md`](clipboardget-a210-source-first-closure.md) (source-first, current); also: [`clipboard-buffer-closure.md`](clipboard-buffer-closure.md), `os3k/file_clipboard_buffer.h` |
| A214 | `ClipboardClear` | mechanically closed; logical/allocation-state clear, not secure erase | [`clipboardclear-a214-source-first-closure.md`](clipboardclear-a214-source-first-closure.md) (source-first, current); also: [`clipboard-buffer-closure.md`](clipboard-buffer-closure.md), `os3k/file_clipboard_buffer.h` |
| A218 | shared clipboard-tag byte getter | mechanically closed as `SYS_A218`; low-byte return only; original enum/name open | [`sys-a218-clipboard-tag-getter-source-first-closure.md`](sys-a218-clipboard-tag-getter-source-first-closure.md) (source-first, current); also: [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A21C | raw/filtered clipboard logical-length query | mechanically closed as `SYS_A21C`; selector 3 computes filtered length | [`sys-a21c-source-first-audit.md`](sys-a21c-source-first-audit.md) (source-first, current); also: [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A220 | clipboard storage/base pointer query | mechanically closed as `SYS_A220`; original name open | [`sys-a220-clipboard-storage-pointer-source-first-closure.md`](sys-a220-clipboard-storage-pointer-source-first-closure.md) (source-first, current); also: [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A224 | clipboard maximum allocated-capacity query | mechanically closed as `SYS_A224`; original name open | [`sys-a224-clipboard-capacity-source-first-closure.md`](sys-a224-clipboard-capacity-source-first-closure.md) (source-first, current); also: [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A228 | allocator-backed clipboard logical-size set/resize | mechanically closed as `SYS_A228`; returns effective clamped size | [`sys-a228-clipboard-resize-source-first-closure.md`](sys-a228-clipboard-resize-source-first-closure.md) (source-first, current); also: [`clipboard-state-capacity-closure.md`](clipboard-state-capacity-closure.md), `os3k/file_clipboard_state.h` |
| A22C | selected SmartApplet runtime-index install after block-mask check | mechanically closed as `SYS_A22C`; original name open | [`applet-selection-closure.md`](applet-selection-closure.md), `os3k/applet_selection.h` |
| A230 | clear selected SmartApplet runtime index to `-1` | mechanically closed as `SYS_A230`; deterministic `-1` result | [`sys-a230-applet-selection-reset-source-first-closure.md`](sys-a230-applet-selection-reset-source-first-closure.md) (source-first, current); also: [`applet-selection-closure.md`](applet-selection-closure.md), `os3k/applet_selection.h` |
| A234 | selected runtime index to 16-bit SmartApplet ID | mechanically closed as `SYS_A234`; low-word contract | [`sys-a234-selected-applet-id-source-first-closure.md`](sys-a234-selected-applet-id-source-first-closure.md) (source-first, current); also: [`applet-selection-closure.md`](applet-selection-closure.md), `os3k/applet_selection.h` |
| A238 | `AppletFindByName` | mechanical A; SDK consolidated/audited; prefix-name lookup; exclusive 32-bit start index; full 32-bit runtime-index result; const input; old 8-bit index/return superseded; regression pending | [`applet-find-by-name-a238-source-first-closure.md`](applet-find-by-name-a238-source-first-closure.md) (source-first, current); also: [`appletfindbyname-closure.md`](appletfindbyname-closure.md), [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md), `os3k/os3k.h` |
| A23C | `AppletFindById` | mechanical A; SDK consolidated/audited; exact 16-bit header-ID lookup; full 32-bit runtime-index result; old 8-bit return superseded; regression pending | [`applet-find-by-id-a23c-source-first-closure.md`](applet-find-by-id-a23c-source-first-closure.md) (source-first, current); also: [`appletfindbyid-closure.md`](appletfindbyid-closure.md), [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md), `os3k/os3k.h` |
| A240 | `AppletGetName` | mechanical A; SDK consolidated/audited; validated 32-bit runtime index; bounded 36-byte header-name copy plus NUL; byte-only 1/0 return; old `int` return superseded; regression pending | [`applet-get-name-a240-source-first-closure.md`](applet-get-name-a240-source-first-closure.md) (source-first, current); also: [`appletgetname-closure.md`](appletgetname-closure.md), [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md), `os3k/os3k.h` |
| A244 | `AppletSendMessage` | mechanical A; SDK consolidated/audited; 32-bit runtime target; target validation; synthetic/private message normalization; OS-owned A5/current-applet context switch and restore; byte-only 1/0 return; regression emulator-first pending | [`applet-send-message-a244-source-first-closure.md`](applet-send-message-a244-source-first-closure.md) (source-first, current); also: [`appletsendmessage-closure.md`](appletsendmessage-closure.md), [`applet-runtime-api-closure.md`](applet-runtime-api-closure.md), `os3k/os3k.h` |
| A248 | `SYS_A248` file-password-protection state getter | mechanical A; SDK consolidated/audited; no arguments; read-only raw low-byte runtime state; original name open; regression pending | [`sys-a248-source-first-closure.md`](sys-a248-source-first-closure.md) (source-first, current); also: [`sys-a248-closure.md`](sys-a248-closure.md), [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h` |
| A24C | `SYS_A24C(password)` master-password comparison | mechanically closed; non-interactive low-byte Boolean result, original name open | [`sys-a24c-master-password-compare-source-first-closure.md`](sys-a24c-master-password-compare-source-first-closure.md) (source-first, current); also: [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h` |
| A250 | `SYS_A250(prompt_variant,optional_text)` interactive master-password gate | mechanical A; firmware consumes the low byte of the first slot; all direct native callers use 2; selector meaning/name remains open | [`sys-a250-master-password-gate-source-first-closure.md`](sys-a250-master-password-gate-source-first-closure.md), [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h`, `os3k/os3k.h` |
| A254 | `SYS_A254(token_group,name_out)` File API token-group/high-byte selector | mechanically closed; selector 0=current, explicit 1..4, optional 30-byte group-name copy | [`sys-a254-file-token-group-source-first-closure.md`](sys-a254-file-token-group-source-first-closure.md) (source-first, current); also: [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/file_token_group.h` |
| A258 | `SYS_A258(protection_state)` file-password-protection state setter | mechanically closed; raw byte store, no return contract or built-in authorization | [`sys-a258-file-password-protection-setter-source-first-closure.md`](sys-a258-file-password-protection-setter-source-first-closure.md) (source-first, current); also: [`password-token-group-runtime-closure.md`](password-token-group-runtime-closure.md), `os3k/password_runtime.h` |
| A25C | `SYS_A25C(action_mask,key)` global-service dispatcher | mechanically closed; 32-bit mask, independent bits 0–3, signed returns 0/8/-9 | [`system-service-dispatch-closure.md`](system-service-dispatch-closure.md), `os3k/os3k.h` |
| A260–A270 | `SYS_A260`/`A264`/`A268`/`A26C`/`A270` selection-block masks | mechanically closed; 32-entry longword table, bit-0 and dynamic-reason mutations | [`applet-selection-mask-closure.md`](applet-selection-mask-closure.md), `os3k/applet_selection_mask.h` |
| A274 | `SYS_A274(void)` printer-selection dialog | mechanically closed; no return contract; AS3000 has 7 printer records and StyleWriter validation, NEO has 5 | [`sys-a274-source-first-closure.md`](sys-a274-source-first-closure.md) (source-first, current); also: [`printer-selection-closure.md`](printer-selection-closure.md), `os3k/printer_selection.h` |
| A278 | `SYS_A278` | see closure | [`applet-current-settings-closure.md`](applet-current-settings-closure.md) (current) |
| A27C | `SYS_A27C(cursor,field_0,field_2,payload_size,payload)` packed-record cursor | mechanically closed; five pointer slots, three 16-bit header outputs, payload pointer and conditional even-byte cursor advance | [`packed-record-cursor-closure.md`](packed-record-cursor-closure.md), `os3k/record_cursor.h` |
| A280 | `SYS_A280(cursor,field_0,field_2,payload_size,payload)` packed-record writer | mechanically closed; five slots, three 16-bit header inputs, exact payload copy and even-byte cursor advance | [`packed-record-writer-closure.md`](packed-record-writer-closure.md), `os3k/record_writer.h` |
| A284 | `SYS_A284(cursor,field_0,field_2,payload_size,payload)` packed-record search | mechanically closed; first exact pair, zero first-field sentinel, optional outputs, signed 0/-1 return | [`packed-record-search-closure.md`](packed-record-search-closure.md), `os3k/record_search.h` |
| A288 | `SYS_A288(void)` transport payload-byte limit | mechanical A; no arguments; returns `(current limit - 6) mod 65536` in D0.W; upper D0 untouched; original name open | [`transport-payload-limit-closure.md`](transport-payload-limit-closure.md), `os3k/transport_payload.h` |
| A28C | `SYS_A28C(primary_name,secondary_name,data_callback,event_callback)` IrDA state initializer | mechanical A; four 32-bit slots, byte 0/0x22 return, mandatory data callback, optional event callback; confirmed secondary-string bounds defect | [`irda-state-init-closure.md`](irda-state-init-closure.md), `os3k/irda_state_init.h` |

## Source layers used for reconstruction

Evidence is normally assembled from several independent layers:

1. **Firmware handlers** from AS3000 System 3 November 2005, NEO System 3
   November 2005, and NEO/System 3.15 July 2013.
2. **Official SmartApplet callers** such as AlphaWord Plus, AlphaQuiz, KeyWords,
   Calculator and ControlPanel.
3. **System 3 internal callers and strings**, especially when an operation is
   encapsulated rather than directly invoked by SmartApplets.
4. **Original AS3000 source/object material (1998–2000)** used as historical
   genealogy, not as automatic proof of modern ABI identity.
5. **Regression probes/matrices** used as executable specifications; runtime
   execution is reported separately from static reconstruction.

## Safety policy

Destructive operations are documented explicitly. File/clipboard/password
experiments should use disposable emulator state before hardware. No ROM image,
proprietary binary, password or user data belongs in this repository.
