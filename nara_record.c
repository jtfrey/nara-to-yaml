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

#include "nara_record.h"
#include "nara_record_impl.h"

#include "nara_classroom_impl.c"
#include "nara_school_impl.c"
#include "nara_district_impl.c"

nara_record_process_fn      __nara_record_process_fns[nara_record_type_max] = {
                                NULL,
                                __nara_record_process_district,
                                __nara_record_process_school,
                                __nara_record_process_classroom
                            };
nara_record_to_yaml_fn      __nara_record_to_yaml_fns[nara_record_type_max] = {
                                NULL,
                                __nara_record_to_yaml_district,
                                __nara_record_to_yaml_school,
                                __nara_record_to_yaml_classroom
                            };
nara_record_destroy_fn      __nara_record_destroy_fns[nara_record_type_max] = {
                                NULL,
                                __nara_record_destroy_district,
                                __nara_record_destroy_school,
                                __nara_record_destroy_classroom
                            };

/**/

iconv_t
__nara_record_iconv_instance(void)
{
    static iconv_t encodingConverter = (iconv_t)-1;
    
    if ( encodingConverter == (iconv_t)-1 ) {
        encodingConverter = iconv_open("ASCII//IGNORE", "EBCDIC-CP-IT");
        if ( encodingConverter == (iconv_t)-1 ) {
            fprintf(stderr, "ERROR:  unable to initialize iconv encoding converter (errno = %d)\n", errno);
            exit(EINVAL);
        }
    }
    return encodingConverter;
}

/**/

nara_record_t*
nara_record_read(
    FILE    *fptr,
    size_t  recordSize
)
{
    nara_record_t   *newRecord = malloc(recordSize);
    
    if ( newRecord ) {
        if ( fread(newRecord, recordSize, 1, fptr) != 1 ) {
            fprintf(stderr, "ERROR:  unable to read full record from file (errno = %d)\n", errno);
            free((void*)newRecord);
            newRecord = NULL;
        } else {
            switch ( (newRecord->recordType = nara_be_to_host_32(newRecord->recordType)) ) {
                case nara_record_type_district:
                case nara_record_type_school:
                case nara_record_type_classroom: {
                    newRecord = __nara_record_process_fns[newRecord->recordType](newRecord, __nara_record_iconv_instance());
                    break;
                }
                default: {
                    fprintf(stderr, "ERROR:  unknown record type %d at %lld\n", newRecord->recordType, (long long int)ftell(fptr));
                    free((void*)newRecord);
                    newRecord = NULL;
                    break;
                }
            }
        }
    }
    return newRecord;
}

/**/

void
nara_record_to_yaml(
    nara_record_t   *theRecord,
    FILE            *fptr
)
{
    switch ( theRecord->recordType ) {
        case nara_record_type_district:
        case nara_record_type_school:
        case nara_record_type_classroom: {
            __nara_record_to_yaml_fns[theRecord->recordType](theRecord, fptr);
            break;
        }
        default: {
            fprintf(stderr, "ERROR:  unknown record type %d\n", theRecord->recordType);
            break;
        }
    }
}

/**/

nara_record_t*
nara_record_destroy(
    nara_record_t   *theRecord
)
{
    switch ( theRecord->recordType ) {
        case nara_record_type_district:
        case nara_record_type_school:
        case nara_record_type_classroom: {
            theRecord = __nara_record_destroy_fns[theRecord->recordType](theRecord);
            break;
        }
        default: {
            fprintf(stderr, "ERROR:  unknown record type %d\n", theRecord->recordType);
            break;
        }
    }
    return theRecord;
}

