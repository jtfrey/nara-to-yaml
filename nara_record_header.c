/*
 * nara_record_header
 *
 * The NARA compressed file is organized into two record tiers.  The second level contains
 * district, school, and classroom records.
 *
 */

#include "nara_record_header.h"

void
nara_record_header_process(
    nara_record_header_t *recordHeader
)
{
    recordHeader->recordLength = nara_be_to_host_16(recordHeader->recordLength);
}
