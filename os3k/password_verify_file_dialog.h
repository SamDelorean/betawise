#ifndef _OS3K_PASSWORD_VERIFY_FILE_DIALOG_H_
#define _OS3K_PASSWORD_VERIFY_FILE_DIALOG_H_

#include <stdint.h>

/* Raw trap declaration. */
uint8_t SYS_A1E0(
    uint16_t file_id,
    uint8_t try_once,
    uint8_t allow_escape,
    uint8_t allow_special_navigation,
    uint8_t allow_file_switching,
    uint16_t *selected_file_out);

/* Historical API name retained as a source-level alias. */
#define PasswordVerifyFileDialog SYS_A1E0

#endif
