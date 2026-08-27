#ifndef _OS3K_FILE_CLIPBOARD_BUFFER_H_
#define _OS3K_FILE_CLIPBOARD_BUFFER_H_

#include <stdint.h>

/*
 * Reconstructed System 3 clipboard buffer ABI.
 *
 * A20C/A210/A214 already carried the BetaWise names ClipboardSet,
 * ClipboardGet and ClipboardClear. Direct firmware analysis now closes their
 * mechanical contracts across AS3000 2005, NEO 2005 and NEO 2013.
 *
 * Exact symbolic names for clipboard_tag and read_variant values have not been
 * recovered. See docs/clipboard-buffer-closure.md.
 */

/* Replace clipboard contents from a caller buffer.
 *
 * buffer == NULL -> raw -6
 * count > 0x7fffffff -> raw -128
 * clipboard_tag == 0 -> raw -129
 *
 * On success the transfer is limited to the clipboard's resulting maximum
 * allocation, current/recoverable size are set to the actual count, the tag is
 * stored, and the actual count is returned.
 */
int32_t ClipboardSet(const uint8_t* buffer,
    uint32_t count,
    uint8_t clipboard_tag);

/* Copy clipboard contents to a caller buffer.
 *
 * buffer == NULL -> raw -6
 * count > 0x7fffffff -> raw -128
 *
 * read_variant == 3 selects the control-code-aware/filtered path; every other
 * value selects raw copying. The raw path returns min(count, clipboard size).
 *
 * Important generation difference: the 2005 AS3000/NEO filtered handlers pass
 * an uninitialized local as the filter helper's initial output limit, whereas
 * NEO 2013 initializes that limit from count and passes the full clipboard size
 * as source length. Do not assume identical selector-3 truncation/output across
 * generations without regression.
 */
int32_t ClipboardGet(uint8_t* buffer,
    uint32_t count,
    uint8_t read_variant);

/* Clear clipboard logical/allocation state.
 *
 * Clears current/recoverable size, cursor and the shared clipboard tag, then
 * invokes allocator/reclaim maintenance. It does not explicitly zero-fill the
 * backing storage and is therefore not a secure erase primitive.
 */
void ClipboardClear(void);

#endif
