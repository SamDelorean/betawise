#ifndef _OS3K_PASSWORD_RUNTIME_H_
#define _OS3K_PASSWORD_RUNTIME_H_

#include <stdint.h>

/*
 * A248: returns the raw low-byte global file-password-protection state.
 * Normal System 3 usage is 0/1; only the low byte of D0 is contractual.
 */
uint8_t SYS_A248(void);

/*
 * A24C: compares a valid NUL-terminated string with the current master
 * password. Returns low-byte 1 on match, 0 otherwise.
 */
uint8_t SYS_A24C(const char *password);

/*
 * A250: interactive master-password gate. The first 32-bit slot is unused by
 * the compared handlers; native System 3 callers pass 2. prompt may be NULL.
 * Correct password returns low-byte 1; Escape/cancel returns 0; a wrong
 * password displays an error, delays two seconds and retries.
 */
uint8_t SYS_A250(uint32_t reserved, const char *prompt);

/*
 * A258: writes the low byte directly to the same global returned by A248.
 * No validation or master-password check is performed here. D0 is untouched,
 * so there is no public return value.
 */
void SYS_A258(uint8_t protection_state);

#endif
