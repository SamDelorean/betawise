#ifndef _OS3K_PRINTER_SELECTION_H_
#define _OS3K_PRINTER_SELECTION_H_

/*
 * Reconstructed System 3 printer-selection UI ABI.
 *
 * A274 is mechanically closed across AS3000 2005, NEO 2005 and NEO 2013,
 * but a reliable original public/vendor name has not been recovered. The
 * generic A-line name is intentionally retained. See
 * docs/printer-selection-closure.md.
 */

/*
 * Opens the System 3 interactive printer-selection dialog.
 *
 * The handler consumes no caller arguments. It maintains a ROM-specific
 * selected-printer index and builds the choice list from the System printer
 * record table. There is no stable caller-visible D0 return contract.
 */
void SYS_A274(void);

#endif
