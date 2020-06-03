/*
 * nara_record_header
 *
 * The NARA compressed file is organized into two record tiers.  The second level contains
 * district, school, and classroom records.
 *
 */

#ifndef __NARA_RECORD_HEADER_H__
#define __NARA_RECORD_HEADER_H__

#include "nara_base.h"

typedef struct {
    uint16_t    recordLength;
    uint16_t    dummy;
} nara_record_header_t;

void nara_record_header_process(nara_record_header_t *recordHeader);

#endif /* __NARA_RECORD_HEADER_H__ */
