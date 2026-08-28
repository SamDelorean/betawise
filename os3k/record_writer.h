#ifndef _OS3K_RECORD_WRITER_H_
#define _OS3K_RECORD_WRITER_H_

#include <stdint.h>

/*
 * Reconstructed System 3 packed-record writer ABI.
 *
 * The original public/vendor symbol and the meanings of the first two record
 * fields have not been recovered, so the neutral A-line name and field labels
 * are intentionally retained.
 *
 * All pointer arguments must be valid. The destination at *cursor must have
 * room for a six-byte header, payload_size bytes, and any logical even-byte
 * padding. The routine performs no bounds, NULL, or overlap checks and does not
 * initialize the padding byte.
 */
void SYS_A280(
    uint8_t **cursor,
    uint16_t field_0,
    uint16_t field_2,
    uint16_t payload_size,
    const uint8_t *payload);

#endif
