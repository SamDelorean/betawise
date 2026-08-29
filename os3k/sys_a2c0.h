#ifndef OS3K_SYS_A2C0_H
#define OS3K_SYS_A2C0_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2C0.
 *
 * Five physical 32-bit stack slots are part of the ABI.
 * `state`, `selector`, `param`, and `status_out` are consumed. The fourth
 * slot is physically present in confirmed callers but is not read by the
 * AS3000 2005, NEO 2005, or NEO 2013 handlers; preserve it as `reserved`
 * for binary compatibility rather than deleting it from the raw signature.
 *
 * `status_out` is mandatory in the observed implementations: it is
 * dereferenced without a NULL check, initialized to zero, and receives
 * complete 32-bit helper status values.
 *
 * D0.L is the contractual return register. The mechanically observed final
 * domain is exactly 0, 1, or 0x02000006. This therefore must not be modeled
 * as a byte or boolean. No vendor meaning is assigned to those values.
 */
uint32_t SYS_A2C0(void *state,
                  uint32_t selector,
                  uint32_t param,
                  uint32_t reserved,
                  uint32_t *status_out);

#endif /* OS3K_SYS_A2C0_H */
