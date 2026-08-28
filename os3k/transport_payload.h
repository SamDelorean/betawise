#ifndef _OS3K_TRANSPORT_PAYLOAD_H_
#define _OS3K_TRANSPORT_PAYLOAD_H_

#include <stdint.h>

/*
 * Returns the current payload-byte limit used by the associated internal
 * transport send path. Only the low 16 bits of D0 are contractual.
 *
 * The original public/vendor symbol and transport name have not been
 * recovered, so the neutral A-line name is intentionally retained.
 *
 * See docs/transport-payload-limit-closure.md.
 */
uint16_t SYS_A288(void);

#endif

