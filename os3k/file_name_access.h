#ifndef _OS3K_FILE_NAME_ACCESS_H_
#define _OS3K_FILE_NAME_ACCESS_H_

#include <stdint.h>

/* Get or set descriptor filename (+0x34, maximum 15 chars plus NUL).
 * set_name == 0 optionally copies to name_buffer and returns the internal name
 * pointer. set_name != 0 validates/normalizes/duplicate-checks the new name.
 * Resolver failure or invalid/duplicate input returns NULL.
 */
char *SYS_A1F0(uint16_t file_id, char *name_buffer, uint8_t set_name);

#endif
