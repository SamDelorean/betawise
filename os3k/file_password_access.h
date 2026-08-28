#ifndef _OS3K_FILE_PASSWORD_ACCESS_H_
#define _OS3K_FILE_PASSWORD_ACCESS_H_

#include <stdint.h>

/*
 * Reconstructed System 3 per-file password access ABI.
 *
 * read_back == 0 writes password into the resolved descriptor. Passwords are
 * limited to five characters plus NUL; length >= 6 returns raw status -8.
 * read_back != 0 copies the descriptor password into caller-provided storage.
 *
 * Returns the canonical resolved file token on success or 0 if resolution
 * fails. The handler provides no safe NULL-pointer path for password.
 */
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);

#endif
