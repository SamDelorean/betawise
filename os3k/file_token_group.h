#ifndef _OS3K_FILE_TOKEN_GROUP_H_
#define _OS3K_FILE_TOKEN_GROUP_H_

#include <stdint.h>

/*
 * A254: resolve an explicit File API token group (1..4), or token_group=0 for
 * the current group (current_group_flags & 0x7F). Returns the effective group
 * byte, or 0 if unavailable/invalid/empty. group_name_out may be NULL; when
 * non-NULL it should provide at least 30 writable bytes for the NUL-terminated
 * group-name copy.
 *
 * System 3 callers use the returned byte as token_high for A1FC token
 * construction/validation.
 */
uint8_t SYS_A254(uint8_t token_group, char *group_name_out);

#endif
