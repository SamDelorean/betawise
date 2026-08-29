#ifndef _OS3K_SYS_A2B4_H_
#define _OS3K_SYS_A2B4_H_

#include <stdint.h>

/*
 * A2B4 copies three internal 32-bit values into three required output
 * pointers. The original vendor names and meanings of those values have not
 * been recovered, so the API intentionally keeps neutral field names.
 *
 * All three pointers must be valid; the firmware performs no NULL checks.
 * The handler does not modify D0 and exposes no public return value.
 */
void SYS_A2B4(uint32_t *out_0, uint32_t *out_1, uint32_t *out_2);

#endif
