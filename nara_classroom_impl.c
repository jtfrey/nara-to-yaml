/*
 * nara_classroom_impl
 *
 * The NARA "classroom" record implementation.
 *
 */

#include "nara_classroom.h"

typedef union {
    nara_classroom_t    break_out;
    struct {
        uint32_t        f1[5 + nara_ethnicity_total];
    } compact;
} nara_classroom_internal_t;

/**/

nara_record_t*
__nara_record_process_classroom(
    nara_record_t*      theRecord,
    iconv_t             encodingConverter
)
{
    nara_classroom_internal_t   *classroom = (nara_classroom_internal_t*)theRecord;
    unsigned int                i, iMax;
    
    iMax = sizeof(classroom->compact.f1) / sizeof(uint32_t);
    for ( i = 1; i < iMax; i ++ ) classroom->compact.f1[i] = nara_be_to_host_32(classroom->compact.f1[i]);
    
    return theRecord;
}

/**/

void
__nara_record_to_yaml_classroom(
    nara_record_t       *theRecord,
    FILE                *fptr
)
{
    nara_classroom_t    *classroom = (nara_classroom_t*)theRecord;
    unsigned int        i, iMax;
    
    fprintf(fptr,
            "- recordType: classroom\n"
            "  schoolSystemCode: %u\n"
            "  schoolCode: %u\n"
            "  gradeLevel: %u\n"
            "  classroomCode: %u\n"
            "  pupilCounts:\n",
            classroom->schoolSystemCode,
            classroom->schoolCode,
            classroom->gradeLevel,
            classroom->classroomCode
        );
    for ( i = 0; i < nara_ethnicity_total; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], classroom->pupilCounts[i]);
}

/**/

nara_record_t*
__nara_record_destroy_classroom(
    nara_record_t       *theRecord
)
{
    free((void*)theRecord);
    return NULL;
}
    