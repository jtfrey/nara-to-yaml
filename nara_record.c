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

#if defined(NARA_1986_FORMAT)
#   define NARA_1986_RECORD_SLOTS   700
#   define NARA_1986_RECORD_SIZE    (sizeof(uint32_t) * NARA_1986_RECORD_SLOTS) + 1
#   include "1986/nara_summary_impl.c"
#   include "1986/nara_school_impl.c"
#   include "1986/nara_district_impl.c"
#elif defined(NARA_1976_FORMAT)
#   define NARA_1976_RECORD_SLOTS   872
#   define NARA_1976_RECORD_SIZE    (sizeof(uint32_t) * NARA_1976_RECORD_SLOTS)
#   include "1976/nara_classroom_impl.c"
#   include "1976/nara_school_impl.c"
#   include "1976/nara_district_impl.c"
#else
#   include "pre-1976/nara_classroom_impl.c"
#   include "pre-1976/nara_school_impl.c"
#   include "pre-1976/nara_district_impl.c"
#endif

nara_record_is_type_fn      __nara_record_is_type_fns[nara_record_type_max] = {
                                NULL,
                                __nara_record_is_type_district,
                                __nara_record_is_type_school,
                                __nara_record_is_type_classroom
                            };
nara_record_process_fn      __nara_record_process_fns[nara_record_type_max] = {
                                NULL,
                                __nara_record_process_district,
                                __nara_record_process_school,
                                __nara_record_process_classroom
                            };
nara_export_init_fn         __nara_export_init_fns[nara_record_type_max] = {
                                NULL,
                                __nara_export_init_district,
                                __nara_export_init_school,
                                __nara_export_init_classroom
                            };
nara_record_export_fn       __nara_record_export_fns[nara_record_type_max] = {
                                NULL,
                                __nara_record_export_district,
                                __nara_record_export_school,
                                __nara_record_export_classroom
                            };
nara_export_destroy_fn      __nara_export_destroy_fns[nara_record_type_max] = {
                                NULL,
                                __nara_export_destroy_district,
                                __nara_export_destroy_school,
                                __nara_export_destroy_classroom
                            };
nara_record_destroy_fn      __nara_record_destroy_fns[nara_record_type_max] = {
                                NULL,
                                __nara_record_destroy_district,
                                __nara_record_destroy_school,
                                __nara_record_destroy_classroom
                            };

/**/

nara_record_t*
nara_record_read(
    FILE    *fptr,
    size_t  recordSize
)
{
    nara_record_t   *newRecord = NULL;
    size_t          bytesRead;
    
    if ( feof(fptr) ) return NULL;

#if defined(NARA_1986_FORMAT)
    recordSize = NARA_1986_RECORD_SIZE;
#elif defined(NARA_1976_FORMAT)
    recordSize = NARA_1976_RECORD_SIZE;
#endif
    newRecord = (nara_record_t*)malloc(recordSize);
    if ( newRecord ) {
        bytesRead = fread(newRecord, 1, recordSize, fptr);
        if ( bytesRead == 0 ) {
            free((void*)newRecord);
            newRecord = NULL;
        }
        else if ( bytesRead < recordSize ) {
            fprintf(stderr, "ERROR:  unable to read full record from file at %lld (expected %lld, got %lld, errno = %d)\n", (long long int)ftell(fptr), (long long int)recordSize, (long long int)bytesRead, errno);
            free((void*)newRecord);
            newRecord = NULL;
        } else {
            uint32_t        recordType = 0;
            
            while ( recordType < nara_record_type_max ) {
                if ( __nara_record_is_type_fns[recordType] && __nara_record_is_type_fns[recordType](newRecord, recordSize) ) break;
                recordType++;
            }
            if ( recordType == nara_record_type_max ) {
                fprintf(stderr, "ERROR:  unknown record type at %lld\n", (long long int)ftell(fptr));
                free((void*)newRecord);
                newRecord = NULL;
            } else {
                newRecord = __nara_record_process_fns[recordType](newRecord);
            }
        }
    }
    return newRecord;
}

/**/

nara_export_context_t
nara_export_init(
    const char  *exportArg
)
{
    nara_export_context_t   outContext = NULL;
    const char              *p = exportArg;
    
    while ( *p && (*p != ':') ) p++;
    if ( *p ) {
        size_t  pLen = p - exportArg;
        
        p++;
        if ( (pLen == 4) && (strncasecmp(exportArg, "yaml", 4) == 0) ) {
            /*
             * Specifier format:
             *
             *   yaml:<filename>
             *
             * where <filename> of "-" implies stdout
             */
            nara_export_context_yaml_t  *context;
            FILE            *fptr;
            
            if ( *p == '\0' ) {
                fptr = NULL;
            }
            else if ( strcmp(p, "-") == 0 ) {
                fptr = stdout;
            }
            else {
                fptr = fopen(p, "w");
                if ( ! fptr ) {
                    fprintf(stderr, "ERROR:  unable to open YAML file for output (errno = %d)\n", errno);
                    goto early_exit;
                }
            }
            context = (nara_export_context_yaml_t*)malloc(sizeof(nara_export_context_yaml_t));
    
            if ( context ) {
                context->base.format = nara_export_format_yaml;
                context->fptr = fptr;
            
                outContext = context;
            } else {
                if ( fptr != stdout ) fclose(fptr);
                fprintf(stderr, "ERROR:  unable to allocate export context\n");
            }
        }
        else if ( (pLen == 3) && (strncasecmp(exportArg, "csv", 3) == 0) ) {
            /*
             * Specifier format:
             *
             *   csv:<district-filename>:<school-filename>:<classroom-filename>
             *
             * where <XXX-filename> of "-" implies stdout and "" does not output that
             * type of record
             */
            nara_export_context_csv_t  *context;
            
            FILE            *districtFptr, *schoolFptr, *classroomFptr;
            char            *filenames = strdup(p), *scanStr;
            char            *token;
            
            if ( ! filenames ) {
                fprintf(stderr, "ERROR:  unable to duplicate filename list in CSV export init\n");
                goto early_exit;
            }
            
            scanStr = filenames;
            token = strsep(&scanStr, ":");
            if ( token == NULL ) {
                free((void*)filenames);
                fprintf(stderr, "ERROR:  incomplete CSV file specifier (district)\n");
                goto early_exit;
            }
            if ( *token == '\0' ) {
                districtFptr = NULL;
            }
            else if ( strcmp(token, "-") == 0 ) {
                districtFptr = stdout;
            }
            else {
                districtFptr = fopen(token, "w");
                if ( ! districtFptr ) {
                    free((void*)filenames);
                    fprintf(stderr, "ERROR:  unable to open district CSV file for output (errno = %d)\n", errno);
                    goto early_exit;
                }
            }
            
            token = strsep(&scanStr, ":");
            if ( token == NULL ) {
                free((void*)filenames);
                if ( districtFptr && (districtFptr != stdout) ) fclose(districtFptr);
                fprintf(stderr, "ERROR:  incomplete CSV file specifier (school)\n");
                goto early_exit;
            }
            if ( *token == '\0' ) {
                schoolFptr = NULL;
            }
            else if ( strcmp(token, "-") == 0 ) {
                schoolFptr = stdout;
            }
            else {
                schoolFptr = fopen(token, "w");
                if ( ! schoolFptr ) {
                    free((void*)filenames);
                    if ( districtFptr && (districtFptr != stdout) ) fclose(districtFptr);
                    fprintf(stderr, "ERROR:  unable to open school CSV file for output (errno = %d)\n", errno);
                    goto early_exit;
                }
            }
            
            token = strsep(&scanStr, ":");
            if ( token == NULL ) {
                free((void*)filenames);
                if ( districtFptr && (districtFptr != stdout) ) fclose(districtFptr);
                if ( schoolFptr && (schoolFptr != stdout) ) fclose(schoolFptr);
                fprintf(stderr, "ERROR:  incomplete CSV file specifier (classroom)\n");
                goto early_exit;
            }
            if ( *token == '\0' ) {
                classroomFptr = NULL;
            }
            else if ( strcmp(token, "-") == 0 ) {
                classroomFptr = stdout;
            }
            else {
                classroomFptr = fopen(token, "w");
                if ( ! classroomFptr ) {
                    free((void*)filenames);
                    if ( districtFptr && (districtFptr != stdout) ) fclose(districtFptr);
                    if ( schoolFptr && (schoolFptr != stdout) ) fclose(schoolFptr);
                    fprintf(stderr, "ERROR:  unable to open classroom CSV file for output (errno = %d)\n", errno);
                    goto early_exit;
                }
            }
            
            /* Go ahead and allocate our context now: */
            context = (nara_export_context_csv_t*)malloc(sizeof(nara_export_context_csv_t));
            if ( context ) {
                context->base.format = nara_export_format_csv;
                context->districtFptr = districtFptr;
                context->schoolFptr = schoolFptr;
                context->classroomFptr = classroomFptr;
                
                outContext = context;
            } else {
                free((void*)filenames);
                if ( districtFptr && (districtFptr != stdout) ) fclose(districtFptr);
                if ( schoolFptr && (schoolFptr != stdout) ) fclose(schoolFptr);
                if ( classroomFptr && (classroomFptr != stdout) ) fclose(classroomFptr);
                fprintf(stderr, "ERROR:  unable to allocate export context\n");
            }
        }
        else {
            fprintf(stderr, "ERROR:  unhandled format in output specifier: %s\n", exportArg);
        }
    } else {
        fprintf(stderr, "ERROR:  invalid output specifier:  format only\n");
    }
    
    if ( outContext ) {
        unsigned        i;
        
        for ( i = 1; i < nara_record_type_max; i++ )
            if ( __nara_export_init_fns[i] ) __nara_export_init_fns[i](outContext);
    }
    
early_exit:
    return outContext;
}

/**/

void
nara_record_export(
    nara_export_context_t   exportContext,
    nara_record_t           *theRecord
)
{
    if ( exportContext ) {
        switch ( theRecord->recordType ) {
            case nara_record_type_district:
            case nara_record_type_school:
            case nara_record_type_classroom: {
                __nara_record_export_fns[theRecord->recordType](exportContext, theRecord);
                break;
            }
            default: {
                fprintf(stderr, "ERROR:  unknown record type %d\n", theRecord->recordType);
                break;
            }
        }
    }
}

/**/

void
nara_export_destroy(
    nara_export_context_t   exportContext
)
{
    if ( exportContext ) {
        nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
        unsigned                    i;
        
        for ( i = 1; i < nara_record_type_max; i++ )
            if ( __nara_export_destroy_fns[i] ) __nara_export_destroy_fns[i](exportContext);
            
        switch ( BASE_CONTEXT->format ) {
        
            case nara_export_format_yaml: {
                nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
                
                if ( CONTEXT->fptr && (CONTEXT->fptr != stdout) ) fclose(CONTEXT->fptr);
                free((void*)exportContext);
                break;
            }
        
            case nara_export_format_csv: {
                nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
                
                if ( CONTEXT->districtFptr && (CONTEXT->districtFptr != stdout) ) fclose(CONTEXT->districtFptr);
                if ( CONTEXT->schoolFptr && (CONTEXT->schoolFptr != stdout) ) fclose(CONTEXT->schoolFptr);
                if ( CONTEXT->classroomFptr && (CONTEXT->classroomFptr != stdout) ) fclose(CONTEXT->classroomFptr);
                free((void*)exportContext);
                break;
            }
        
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

