#ifndef _OS3K_IRDA_STATE_INIT_H_
#define _OS3K_IRDA_STATE_INIT_H_

#include <stdint.h>

typedef void (*SYS_A28C_DataCallback)(uint8_t *data, uint32_t remaining);
typedef void (*SYS_A28C_EventCallback)(uint32_t event_code);

/*
 * Initializes the singleton IrDA state used by correlated IrCOMM/IrLMP/TinyTP
 * paths. The first two names and data_callback are mandatory; event_callback
 * may be null. Returns 0 on success or 0x22 on rejected inputs.
 *
 * The second-string copy has a confirmed cross-ROM bounds defect. See
 * docs/irda-state-init-closure.md before use.
 */
uint8_t SYS_A28C(
    const char *primary_name,
    const char *secondary_name,
    SYS_A28C_DataCallback data_callback,
    SYS_A28C_EventCallback event_callback);

#endif
