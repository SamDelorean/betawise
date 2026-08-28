#ifndef _OS3K_FILE_PASSWORD_RESET_H_
#define _OS3K_FILE_PASSWORD_RESET_H_

#include <stdint.h>

/*
 * Reset every file password in the current namespace to the factory default
 * after interactive master-password verification.
 *
 * Returns the number of passwords reset, or 0 on cancel/failure.
 * This operation mutates all file passwords in the selected namespace.
 */
uint32_t SYS_A1D8(void);

#endif
