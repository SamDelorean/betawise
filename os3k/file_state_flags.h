#ifndef _OS3K_FILE_STATE_FLAGS_H_
#define _OS3K_FILE_STATE_FLAGS_H_

#include <stdint.h>

/*
 * Mutate supported bits in a resolved file descriptor's state field.
 *
 * AS3000/NEO 2005 support low-byte bits 0x01, 0x04 and 0x08.
 * NEO 2013 also supports 0x10. enable == 1 sets the requested bits;
 * any other value clears them.
 *
 * Returns the canonical file token on success, 0 on resolver failure,
 * or raw System 3 status -14 when no supported mask bit is requested.
 */
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);

#endif
