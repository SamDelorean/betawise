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
 * A250: interactive master-password gate. Firmware reads the low byte of the
 * first ABI slot and passes it into the prompt/UI setup path; all six native
 * direct callers in each canonical ROM pass 2. optional_text may be NULL.
 * Correct password returns low-byte 1; cancel/non-affirmative exit returns 0;
 * a wrong password displays an error/delay sequence and retries.
 */
uint8_t SYS_A250(uint8_t prompt_variant, const char *optional_text);

/*
 * A258: writes the low byte directly to the same global returned by A248.
 * No validation or master-password check is performed here. D0 is untouched,
 * so there is no public return value.
 */
void SYS_A258(uint8_t protection_state);

#endif
