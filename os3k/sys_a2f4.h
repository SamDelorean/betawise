#ifndef OS3K_SYS_A2F4_H
#define OS3K_SYS_A2F4_H

#include <stdint.h>

/*
 * Neutral OS3K ABI reconstruction for A2F4.
 *
 * Official callers construct exactly five physical 32-bit stack slots.
 * The firmware treats state as a pointer, consumes message and param as
 * longword slots, forwards arg4 opaquely to downstream handlers, and requires
 * out_value to be writable (it is cleared on entry and written on one path).
 *
 * D0.L is contractual: an official caller tests it immediately after the
 * call, and the handler explicitly returns the raw values 0 or 1.  No vendor
 * names are assigned here to those values, the message domain, arg4, or the
 * output value.
 */
uint32_t SYS_A2F4(void *state,
                  uint32_t message,
                  uint32_t param,
                  void *arg4,
                  uint32_t *out_value);

#endif /* OS3K_SYS_A2F4_H */
