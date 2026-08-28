#ifndef _OS3K_FILE_STATE_QUERY_H_
#define _OS3K_FILE_STATE_QUERY_H_

#include <stdint.h>

/*
 * Query supported bits in a resolved file descriptor's state field.
 *
 * Supported low-byte mask bits:
 *   AS3000/NEO 2005: 0x01 | 0x04 | 0x08
 *   NEO 2013:        0x01 | 0x04 | 0x08 | 0x10
 *
 * Returns 1 when any requested supported bit is set, 0 when none is set or
 * file resolution fails, and raw System 3 status -14 when the mask contains
 * no supported low-byte state bit.
 */
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);

#endif
