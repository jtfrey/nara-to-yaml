/*
 * nara_state_header
 *
 * The NARA compressed file is organized into two record tiers.  The first level groups
 * all records associated with a single U.S. state.
 *
 */

#ifndef __NARA_STATE_HEADER_H__
#define __NARA_STATE_HEADER_H__

#include "nara_base.h"

typedef struct {
    uint16_t    recordLength;
    uint16_t    dummy;
} nara_state_header_t;

void nara_state_header_process(nara_state_header_t *stateHeader);

#endif /* __NARA_STATE_HEADER_H__ */
