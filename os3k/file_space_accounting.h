#ifndef _OS3K_FILE_SPACE_ACCOUNTING_H_
#define _OS3K_FILE_SPACE_ACCOUNTING_H_

#include <stdint.h>

/*
 * Reconstructed System 3 global/reclaimable file-storage accounting ABI.
 *
 * The original public/vendor symbols have not been recovered, so the neutral
 * A-line names are intentionally retained.
 */

/*
 * Returns a 32-bit storage/character count using System 3 allocator accounting,
 * including reclaimable 0x200-byte regions and the unallocated storage tail.
 */
uint32_t SYS_A1AC(void);

/*
 * Resolves file_id in the current namespace and returns max_size-current_size.
 * Resolution failure returns 0. The operation is read-only.
 */
uint32_t SYS_A1B0(uint16_t file_id);

/*
 * Resolves file_id in the current namespace and returns descriptor max_size.
 * Resolution failure returns 0. The operation is read-only.
 */
uint32_t SYS_A1BC(uint16_t file_id);

#endif
