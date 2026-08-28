#ifndef _OS3K_PASSWORD_CHANGE_FILE_DIALOG_H_
#define _OS3K_PASSWORD_CHANGE_FILE_DIALOG_H_

#include <stdint.h>

/* Raw trap declaration. A resolved dialog exit, including cancellation,
 * returns the canonical token; resolver failure returns the raw negative status.
 */
int32_t SYS_A1E4(uint16_t file_id);

/* Historical API name retained as a source-level alias. */
#define PasswordChangeFileDialog SYS_A1E4

#endif
