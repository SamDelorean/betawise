#ifndef _OS3K_RECORD_CURSOR_H_
#define _OS3K_RECORD_CURSOR_H_

#include <stdint.h>

/*
 * Reconstructed System 3 packed-record cursor ABI.
 *
 * The original public/vendor symbol and the meanings of the first two record
 * fields have not been recovered, so the neutral A-line name and field labels
 * are intentionally retained.
 *
 * All five pointer arguments must be valid. *cursor must address a record with
 * three big-endian 16-bit fields followed by its payload. The routine performs
 * no bounds or NULL checks.
 */
void SYS_A27C(
    const uint8_t **cursor,
    uint16_t *field_0_out,
    uint16_t *field_2_out,
    uint16_t *payload_size_out,
    const uint8_t **payload_out);

#endif
