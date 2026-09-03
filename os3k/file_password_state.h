#ifndef _OS3K_FILE_PASSWORD_STATE_H_
#define _OS3K_FILE_PASSWORD_STATE_H_

#include <stdint.h>

/*
 * Reconstructed System 3 file password/state ABI.
 *
 * These interfaces intentionally retain their A-line names. Their mechanical
 * contracts are closed, but reliable original System 3 public symbols have not
 * been recovered. See docs/file-password-state-closure.md.
 */

/*
 * Mutate bits in the resolved file descriptor's state field.
 *
 * Supported low-byte mask bits:
 *   AS3000/NEO 2005: 0x01 | 0x04 | 0x08
 *   NEO 2013:        0x01 | 0x04 | 0x08 | 0x10
 *
 * enable == 1 sets the requested bits; every other value clears them.
 * Returns the canonical resolved file token on success, 0 on resolver failure,
 * or raw System 3 status -14 when no supported low-byte mask bit is present.
 */
int32_t SYS_A1D0(uint16_t file_id, uint32_t state_mask, uint8_t enable);

/*
 * Query bits in the same per-file state field used by SYS_A1D0.
 * Returns 1 when any requested bit is set, 0 when none is set/resolution fails,
 * or raw System 3 status -14 for an unsupported mask.
 */
int32_t SYS_A1D4(uint16_t file_id, uint32_t state_mask);

/*
 * Interactive, destructive reset of every file password in the current file
 * namespace to the factory default "write" after master-password verification.
 * Returns the number of passwords reset, or 0 on cancel/failure.
 *
 * Use emulator/disposable state for regression. This is not a safe hardware
 * discovery probe.
 */
uint32_t SYS_A1D8(void);

/*
 * Bidirectional file-password access. Direct source-first revalidation of the
 * modern System 3 handler shows that write mode accepts strlen(password) <= 6;
 * only lengths greater than six return raw status -8. This differs from the
 * five-character historical PasswordModule limit and must not be projected
 * onto the modern ABI.
 *
 * read_back == 0: copy password into the descriptor; length > 6 returns -8.
 * read_back != 0: copy the descriptor password into caller-provided storage.
 *
 * Returns the canonical resolved file token on success or 0 if resolution
 * fails. The handler does not provide a safe NULL-pointer path for password.
 */
int32_t SYS_A1DC(uint16_t file_id, char *password, uint8_t read_back);

#endif
