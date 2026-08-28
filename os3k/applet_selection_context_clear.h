#ifndef _OS3K_APPLET_SELECTION_CONTEXT_CLEAR_H_
#define _OS3K_APPLET_SELECTION_CONTEXT_CLEAR_H_

#include <stdint.h>

/* Clear the selected SmartApplet runtime context to sentinel -1.
 * The handler deterministically returns -1 in D0.
 */
int32_t SYS_A230(void);

#endif
