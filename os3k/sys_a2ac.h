#ifndef OS3K_SYS_A2AC_H
#define OS3K_SYS_A2AC_H

#include <stdint.h>

/* Neutral OS3K ABI reconstruction for A2AC.
 *
 * One 32-bit pointer slot is consumed. On the common path a helper-generated
 * byte is written to *out_byte, shared state is reset, and the routine returns
 * 0. AS3000 has an additional hardware-specific preamble that may return the
 * full signed 32-bit value -1302 before the common path; in that case the
 * output byte is not guaranteed to have been written. NEO 2005/2013 do not
 * contain that preamble and return 0 on their only path.
 *
 * No vendor function/error name has been recovered.
 */
int32_t SYS_A2AC(uint8_t *out_byte);

#endif /* OS3K_SYS_A2AC_H */
