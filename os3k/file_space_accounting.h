#ifndef _OS3K_FILE_SPACE_ACCOUNTING_H_
#define _OS3K_FILE_SPACE_ACCOUNTING_H_

#include <stdint.h>

/*
 * Reconstructed System 3 per-file maximum-capacity ABI.
 *
 * The original public/vendor symbol has not been recovered, so the neutral
 * A-line name is intentionally retained.
 *
 * Resolves file_id in the current namespace and returns descriptor max_size.
 * Resolution failure returns 0. The operation is read-only.
 */
uint32_t SYS_A1BC(uint16_t file_id);

#endif
