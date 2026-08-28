#ifndef _OS3K_RECORD_SEARCH_H_
#define _OS3K_RECORD_SEARCH_H_

#include <stdint.h>

/*
 * Reconstructed System 3 packed-record search ABI.
 *
 * The original public/vendor symbol and the meanings of the first two record
 * fields have not been recovered, so the neutral A-line name and field labels
 * are intentionally retained.
 *
 * cursor and *cursor must be valid and the sequence must contain a field_0 == 0
 * terminator. The two output pointers may independently be NULL. The routine
 * performs no bounds, alignment, or sequence validation.
 *
 * Returns 0 for the first exact field pair match and -1 at the terminator.
 * See docs/packed-record-search-closure.md.
 */
int32_t SYS_A284(
    const uint8_t **cursor,
    uint16_t field_0,
    uint16_t field_2,
    uint16_t *payload_size_out,
    const uint8_t **payload_out);

#endif
