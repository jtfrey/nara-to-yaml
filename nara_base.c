/*
 * nara_base
 *
 * Baseline includes, macros, and functions.
 *
 */

#include "nara_base.h"

uint16_t
__nara_be_to_le_16(
    uint16_t    value
)
{
    return __builtin_bswap16(value);
}

uint32_t
__nara_be_to_le_32(
    uint32_t    value
)
{
    return __builtin_bswap32(value);
}

/**/

uint16_t
__nara_be_to_be_16(
    uint16_t    value
)
{
    return value;
}

uint32_t
__nara_be_to_be_32(
    uint32_t    value
)
{
    return value;
}

/**/

nara_be_to_host_16_fn nara_be_to_host_16 = __nara_be_to_be_16;
nara_be_to_host_32_fn nara_be_to_host_32 = __nara_be_to_be_32;

/**/

void
nara_endian_init(void)
{
    int     x = 1;
    
    if ( *(char*)&x ) {
        nara_be_to_host_16 = __nara_be_to_le_16;
        nara_be_to_host_32 = __nara_be_to_le_32;
    } else {
        nara_be_to_host_16 = __nara_be_to_be_16;
        nara_be_to_host_32 = __nara_be_to_be_32;
    }
}