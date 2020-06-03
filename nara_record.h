/*
 * nara_record
 *
 * The base of the NARA record format consists of a 32-bit integer indicating
 * the record type.
 *
 * We have an abstract funciton that reads the entire record from the file
 * and performs all the internal cleanup -- byte-swapping, text conversion from
 * EBCDIC.  Other functions are present to write the record as YAML and to
 * destroy the record.
 *
 * Given the recordType, a nara_record_t* can be type cast to the approriate
 * data structure for access by code outside this class cluster.
 *
 */

#ifndef __NARA_RECORD_H__
#define __NARA_RECORD_H__

#include "nara_base.h"

enum {
    nara_record_type_district = 1,
    nara_record_type_school = 2,
    nara_record_type_classroom = 3,
    nara_record_type_max
};

typedef struct {
    uint32_t    recordType;
} nara_record_t;

nara_record_t* nara_record_read(FILE *fptr, size_t recordSize);
void nara_record_to_yaml(nara_record_t *theRecord, FILE *fptr);
nara_record_t* nara_record_destroy(nara_record_t *theRecord);

#endif /* __NARA_RECORD_H__ */
