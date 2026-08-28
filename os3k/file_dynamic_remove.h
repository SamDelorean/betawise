#ifndef _OS3K_FILE_DYNAMIC_REMOVE_H_
#define _OS3K_FILE_DYNAMIC_REMOVE_H_

#include <stdint.h>

/* Remove a file descriptor and reclaim/compact its allocation and table entry.
 * Success returns 0; resolver failure returns the resolver status.
 * Destructive: removes the descriptor itself, not merely its contents.
 */
int32_t SYS_A1EC(uint16_t file_id);

#endif
