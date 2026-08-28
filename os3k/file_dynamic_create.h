#ifndef _OS3K_FILE_DYNAMIC_CREATE_H_
#define _OS3K_FILE_DYNAMIC_CREATE_H_

#include <stdint.h>

/* Create a dynamic file descriptor. Effective maximum allocation is
 * max(requested_max_size, min_size, 0x200).
 * Returns the canonical token on success. Known raw errors: -9 table full,
 * -10 invalid/unavailable group, -12 allocation failure, -13 invalid name,
 * -15 duplicate name.
 */
int32_t SYS_A1E8(uint8_t token_group, const char *name,
                 uint32_t min_size, uint32_t requested_max_size);

#endif
