#ifndef _OS3K_FILE_SPACE_ACCOUNTING_H_
#define _OS3K_FILE_SPACE_ACCOUNTING_H_

#include <stdint.h>

/*
 * Reconstructed System 3 global/reclaimable file-storage accounting ABI.
 *
 * The original public/vendor symbol has not been recovered, so the neutral
 * A-line name is intentionally retained.
 *
 * Returns a 32-bit storage/character count using System 3 allocator accounting,
 * including reclaimable 0x200-byte regions and the unallocated storage tail.
 */
uint32_t SYS_A1AC(void);

#endif
