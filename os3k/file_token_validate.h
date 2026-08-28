#ifndef _OS3K_FILE_TOKEN_VALIDATE_H_
#define _OS3K_FILE_TOKEN_VALIDATE_H_

#include <stdint.h>

/* Build a 16-bit file token from low/high bytes and validate it through the
 * shared resolver. Returns the token on success or the resolver status on
 * failure.
 */
int32_t SYS_A1FC(uint8_t token_low, uint8_t token_high);

#endif
