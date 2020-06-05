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

int
__nara_record_is_type_classroom(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == 40) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_classroom) );
}

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
    for ( i = 0; i < iMax; i ++ ) classroom->compact.f1[i] = nara_be_to_host_32(classroom->compact.f1[i]);
    
    return theRecord;
}

/**/

void
__nara_export_init_classroom(
    nara_export_context_t   exportContext
)
{
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i;
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            break;
        }
        
        case nara_export_format_csv: {
            nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
            
            /*
             * Write column headers to the file:
             */
            if ( CONTEXT->classroomFptr ) {
                fprintf(CONTEXT->classroomFptr,
                        "schoolSystemCode,"
                        "schoolCode,"
                        "gradeLevel,"
                        "classroomCode,"
                    );
                for ( i = 0; i < nara_ethnicity_total - 1; i++ )
                    fprintf(CONTEXT->classroomFptr, "pupilCounts_%s,", nara_ethnicity_labels[i]);
                fprintf(CONTEXT->classroomFptr, "pupilCounts_%s\n", nara_ethnicity_labels[i]);
            }
            break;
        }
    }
}

/**/

void
__nara_export_destroy_classroom(
    nara_export_context_t   exportContext
)
{
}

/**/

void
__nara_record_export_classroom(
    nara_export_context_t   exportContext,
    nara_record_t           *theRecord
)
{
    nara_classroom_t            *classroom = (nara_classroom_t*)theRecord;
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i;
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
            
            if ( CONTEXT->fptr ) {
                fprintf(CONTEXT->fptr,
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
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], classroom->pupilCounts[i]);
            }
            break;
        }
        
        case nara_export_format_csv: {
            nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
            
            if ( CONTEXT->classroomFptr ) {
                fprintf(CONTEXT->classroomFptr,
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,",
                        classroom->schoolSystemCode,
                        classroom->schoolCode,
                        classroom->gradeLevel,
                        classroom->classroomCode
                    );
                for ( i = 0; i < nara_ethnicity_total - 1; i++ )
                    fprintf(CONTEXT->classroomFptr, "%u,", nara_ethnicity_labels[i], classroom->pupilCounts[i]);
                fprintf(CONTEXT->classroomFptr, "%u\n", nara_ethnicity_labels[i], classroom->pupilCounts[i]);
            }
            break;
        }
    }
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
    