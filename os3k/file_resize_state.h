#ifndef _OS3K_FILE_RESIZE_STATE_H_
#define _OS3K_FILE_RESIZE_STATE_H_

#include <stdint.h>

/*
 * Reconstructed System 3 active-file resize/state ABI.
 *
 * Non-negative values request a new size for the active descriptor. The
 * request is clamped to max_size, cursor is clamped to the effective size,
 * and previous_size is raised when needed. Negative values select confirmed
 * state-management commands documented in the SDK notes.
 */
uint32_t SYS_A1B8(int32_t size_or_command);

#endif
