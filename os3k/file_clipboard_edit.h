#ifndef _OS3K_FILE_CLIPBOARD_EDIT_H_
#define _OS3K_FILE_CLIPBOARD_EDIT_H_

#include <stdint.h>

/*
 * Reconstructed System 3 clipboard editing ABI.
 *
 * Functional roles are mechanically closed, but original public System 3 names
 * and enum labels have not been recovered. The A-line names are therefore kept.
 * See docs/clipboard-edit-closure.md.
 */

/* Copy count bytes from active storage+cursor into the clipboard.
 * clipboard_tag must be non-zero. Returns actual bytes copied after clipboard
 * capacity limiting. Known raw errors: -3 no active file, -128 invalid/range
 * count, -129 zero tag. Source data/cursor are not modified.
 */
int32_t SYS_A200(uint32_t count, uint8_t clipboard_tag);

/* Cut count bytes from the active file into the clipboard.
 * Uses the same validation as A200, copies actual=min(requested, clipboard
 * capacity), then calls FileDeleteChars(actual). Thus uncopied bytes are never
 * deleted. Returns actual bytes copied/deleted.
 */
int32_t SYS_A204(uint32_t count, uint8_t clipboard_tag);

/* Paste clipboard contents through the A198 write family.
 * write_mode: 1 append-at-EOF, 2 insert-at-cursor, 3 overwrite-at-cursor;
 * other values reach the underlying raw -5 mode error.
 * read_variant == 3 selects the control-code-aware/filtered source path;
 * other values select the raw source-copy path.
 * Returns the selected underlying writer result.
 */
int32_t SYS_A208(uint8_t write_mode, uint8_t read_variant);

#endif
