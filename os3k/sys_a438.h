#ifndef OS3K_SYS_A438_H
#define OS3K_SYS_A438_H

/*
 * Neutral OS3K ABI reconstruction for A438 / index 270.
 *
 * NEO 2013 only. AS3000 2005 and NEO 2005 do not expose a callable
 * pointer at this table index.
 *
 * The NEO 2013 wrapper copies a NUL-terminated string from firmware-owned
 * storage into dst using the reconstructed OS3K strcpy implementation and
 * returns dst in D0.L. No destination capacity or NULL check is supplied by
 * the wrapper. The semantic meaning of the firmware-owned source is unknown.
 */
char *SYS_A438(char *dst);

#endif /* OS3K_SYS_A438_H */
