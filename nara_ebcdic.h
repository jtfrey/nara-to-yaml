/*
 * nara_ebcdic
 *
 * Built-in EBCDIC to ASCII transcoding.
 *
 */

#ifndef __NARA_EBCDIC_H__
#define __NARA_EBCDIC_H__

#include <stdlib.h>

char nara_ebcdic_to_ascii(char c);
size_t nara_ebcdic_to_ascii_inplace(char *s, size_t sLen);
size_t nara_ebcdic_to_ascii_copy(const char *from, size_t fromLen, char *to, size_t toLen);

#endif /* __NARA_EBCDIC_H__ */
