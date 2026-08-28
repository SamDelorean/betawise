#ifndef _OS3K_FILE_SPACE_ACCOUNTING_H_
#define _OS3K_FILE_SPACE_ACCOUNTING_H_

#include <stdint.h>

/*
 * Reconstructed System 3 file-size/accounting query ABI.
 *
 * The original public/vendor symbol has not been recovered, so the neutral
 * A-line name is intentionally retained.
 *
 * Ordinary 16-bit tokens return current_size. Selectors 0xFC..0xFF access
 * confirmed active/global accounting variants documented by the SDK notes.
 */
uint32_t SYS_A1B4(uint16_t file_id_or_selector);

#endif
