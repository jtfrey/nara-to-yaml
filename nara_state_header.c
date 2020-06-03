/*
 * nara_state_header
 *
 * The NARA compressed file is organized into two record tiers.  The first level groups
 * all records associated with a single U.S. state.
 *
 */

#include "nara_state_header.h"

void
nara_state_header_process(
    nara_state_header_t *stateHeader
)
{
    stateHeader->recordLength = nara_be_to_host_16(stateHeader->recordLength);
}
