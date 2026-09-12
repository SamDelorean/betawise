# SDK audit — public-header coverage gap A200–A234

Status: **AUDIT_FINDING / NEXT_BUILD_BATCH**

## Scope

This pass compares the already reconstructed clipboard/selection contracts against the umbrella public header `os3k/os3k.h`. It does not reopen the ABI and does not strengthen any `CERRADA_CON_LÍMITE_DE_EVIDENCIA` entry.

## Finding

The branch already contains mechanically closed auxiliary declarations for the A200–A234 block, but the umbrella header currently jumps from the File API/password-dialog area to the A238 SmartApplet runtime functions. Therefore these closed services are usable only by including their auxiliary headers directly.

Closed auxiliary contracts presently include:

```c
/* os3k/file_clipboard_edit.h */
int32_t SYS_A200(uint32_t count, uint8_t clipboard_tag);
int32_t SYS_A204(uint32_t count, uint8_t clipboard_tag);
int32_t SYS_A208(uint8_t write_mode, uint8_t read_variant);

/* os3k/file_clipboard_buffer.h */
int32_t ClipboardSet(const uint8_t *buffer, uint32_t count, uint8_t clipboard_tag);
int32_t ClipboardGet(uint8_t *buffer, uint32_t count, uint8_t read_variant);
void ClipboardClear(void);

/* os3k/file_clipboard_state.h */
uint8_t  SYS_A218(void);
uint32_t SYS_A21C(uint8_t read_variant);
uint8_t *SYS_A220(void);
uint32_t SYS_A224(void);
uint32_t SYS_A228(uint32_t requested_size);

/* os3k/applet_selection.h */
uint32_t SYS_A22C(uint32_t applet_index);
int32_t  SYS_A230(void);
uint16_t SYS_A234(void);
```

The ABI reconstruction index describes these contracts as mechanically closed. Original symbolic names remain unknown for the neutral `SYS_Axxx` entries, so no descriptive renaming is proposed.

## Why this is a public-SDK consistency issue

`os3k/syscall.c` already emits veneers for A200 through A234. The auxiliary headers already provide the C types and safety notes. `os3k/os3k.h`, however, exposes A238/A23C/A240/A244 immediately after the older File API/password surface and does not currently declare the A200–A234 block.

This creates an inconsistent umbrella API: applications including only `os3k.h` can call the later A238–A244 applet runtime functions but not the preceding closed clipboard/selected-applet services without knowing the auxiliary-header layout.

## Recommended correction

Treat A200–A234 as one small umbrella-header reconciliation batch. Add the exact existing auxiliary prototypes to `os3k/os3k.h` without changing names, semantics or implementation. Keep the auxiliary headers as focused documentation/interfaces; duplicate declarations are acceptable when signatures are identical.

Do **not** fold A1AC–A1FC into this batch merely because related auxiliary headers exist. Those ranges have stricter consolidation/evidence-limit considerations and should remain separately gated.

## Acceptance gate

No header source change is claimed by this audit note. Integration should occur only when the Linux/m68k build gate is available:

1. add only the exact A200–A234 prototypes already established in auxiliary headers;
2. `make -C os3k clean all` — PASS;
3. clean build/link of one representative applet — PASS;
4. verify no declaration mismatch between umbrella and auxiliary headers;
5. record the result in ABI consolidation/continuity documentation.

Until those gates run, status remains **AUDIT_FINDING / NEXT_BUILD_BATCH**, not `BUILD_VALIDATED`.
