#ifndef _OS3K_FILE_LOCAL_INDEX_LOOKUP_H_
#define _OS3K_FILE_LOCAL_INDEX_LOOKUP_H_

#include <stdint.h>

/* Reverse local-index lookup in the current file group/class.
 * Returns the canonical token when found or raw -11 when no match exists.
 */
int32_t SYS_A1F8(uint16_t local_index);

#endif
