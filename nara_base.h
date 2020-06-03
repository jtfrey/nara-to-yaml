/*
 * nara_base
 *
 * Baseline includes, macros, and functions.
 *
 */

#ifndef __NARA_BASE_H__
#define __NARA_BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

/*!
    @typedef nara_be_to_host_16_fn
    
    Type of a function that performs 16-bit byte-swapping
    from big-endian to the native system endian.
 */
typedef uint16_t (*nara_be_to_host_16_fn)(uint16_t);

/*!
    @typedef nara_be_to_host_32_fn
    
    Type of a function that performs 32-bit byte-swapping
    from big-endian to the native system endian.
 */
typedef uint32_t (*nara_be_to_host_32_fn)(uint32_t);

/*!
    @function nara_endian_init
    
    The functions herein default to assuming a big endian
    runtime.  The main program calls this function to test the
    endianness of the system and choose the appropriate byte-
    swapping functions.
 */
void nara_endian_init(void);

/*!
    @function nara_be_to_host_16
    
    Once nara_endian_init() has been called, this function performs the
    byte-swapping necessary to reorder a 16-bit big-endian integer to
    the system's endianness.
 */
extern nara_be_to_host_16_fn    nara_be_to_host_16;

/*!
    @function nara_be_to_host_32
    
    Once nara_endian_init() has been called, this function performs the
    byte-swapping necessary to reorder a 32-bit big-endian integer to
    the system's endianness.
 */
extern nara_be_to_host_32_fn    nara_be_to_host_32;

#endif /* __NARA_BASE_H__ */
