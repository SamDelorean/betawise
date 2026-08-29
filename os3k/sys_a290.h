#ifndef _OS3K_SYS_A290_H_
#define _OS3K_SYS_A290_H_

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint32_t field_0;
    uint8_t field_4;
    uint8_t field_5;
} SYS_A290_Record;

/*
 * Normalizes/deduplicates exactly six writable six-byte records.
 * optional_out32 may be NULL; when non-NULL it receives exactly 32 bytes.
 * Only the low byte of D0 is contractual.
 *
 * The original vendor symbol and field semantics are not recovered, so the
 * neutral A-line name and structural field names are intentionally retained.
 * See docs/sys-a290-closure.md.
 */
uint8_t SYS_A290(SYS_A290_Record records[6], uint8_t optional_out32[32]);

#endif
