/*
 * nara_summary_impl
 *
 * The NARA "summary" record implementation.
 *
 */

#include "nara_summary.h"


typedef union {
    nara_summary_t      break_out;
    struct {
        uint32_t        f1[3];
        char            systemName[32];
        char            systemStreetAddress[32];
        char            systemCounty[16];
        char            systemCity[16];
        char            systemStateAbbrev[4];
        char            systemZipCode[8];
        uint32_t        f2[2];
        float           f3;
        uint32_t        f4;
        float           f5;
        uint32_t        f6[1 + nara_pupils_max * nara_ethnicity_max + 2 + nara_classroom_survey_slots * nara_classroom_survey_max + 2 + nara_special_ed_max * nara_special_ed_category_max + 2 + nara_selected_course_max * nara_selected_course_category_max + 1 + nara_ethnicity_max + 7];
        uint32_t        unused1[5];
        uint32_t        f7[1 + nara_classroom_survey_slots_additional * nara_classroom_survey_max];
        uint32_t        unused2[8];
        uint32_t        f8[114];
        uint32_t        unused3[24];
    } compact;
} nara_summary_internal_t;

/**/

int
__nara_record_is_type_summary(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == NARA_1986_RECORD_SIZE) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_classroom) );
}

/**/

nara_record_t*
__nara_record_process_summary(
    nara_record_t*      theRecord
)
{
    nara_summary_internal_t     *summary = (nara_summary_internal_t*)theRecord;
    char                        convBuffer[64];
    char                        *fromPtr, *toPtr;
    size_t                      fromLen, toLen;
    
    unsigned int                i, iMax;
    
    iMax = sizeof(summary->compact.f1) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) summary->compact.f1[i] = nara_be_to_host_32(summary->compact.f1[i]);
    
    iMax = sizeof(summary->compact.f2) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) summary->compact.f2[i] = nara_be_to_host_32(summary->compact.f2[i]);
    
    summary->compact.f3 = nara_be_to_host_float(summary->compact.f3);
    
    summary->compact.f4 = nara_be_to_host_32(summary->compact.f4);
    
    summary->compact.f5 = nara_be_to_host_float(summary->compact.f5);
    
    iMax = sizeof(summary->compact.f6) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) summary->compact.f6[i] = nara_be_to_host_32(summary->compact.f6[i]);
    
    iMax = sizeof(summary->compact.f7) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) summary->compact.f7[i] = nara_be_to_host_32(summary->compact.f7[i]);
    
    iMax = sizeof(summary->compact.f8) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) summary->compact.f8[i] = nara_be_to_host_32(summary->compact.f8[i]);
    
    TRANSCODE(summary, systemName, "district name");
    TRANSCODE(summary, systemStreetAddress, "district street address");
    TRANSCODE(summary, systemCounty, "district county");
    TRANSCODE(summary, systemCity, "district city");
    TRANSCODE(summary, systemStateAbbrev, "district state abbreviation");
    TRANSCODE(summary, systemZipCode, "district zip code");
    
    return theRecord;
}

/**/

void
__nara_export_init_summary(
    nara_export_context_t   exportContext
)
{
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i, j;
    
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
                        "systemOECode,"
                        "selectionCode,"
                        "systemName,"
                        "systemStreetAddress,"
                        "systemCounty,"
                        "systemCity,"
                        "systemStateAbbrev,"
                        "systemZipCode,"
                        "numSchoolsInSchoolSystem,"
                        "numSchoolsReporting,"
                        "sampleWeight,"
                        "isSubSampledDistrict,"
                        "subSampledWeight,"
                        "isSubSampledSchool,"
                        
                        "schoolShouldHaveCompletedPart6,"
                        "numberOfClassroomsSurveyed,"
                        "isSpecialEdProgramNotOffered,"
                        "isItem7Completed,"
                        "isSection3Completed,"
                        "shouldSchoolHaveCompletedItem8,"
                        "shouldSchoolHaveCompletedItem9,"
                        
                        "isCourtOrderYesFederal,"
                        "isCourtOrderYesState,"
                        "isCourtOrderNo,"
                        "childrenAwaitingInitEval,"
                        "childrenRequireSpecialEd,"
                        "childrenReceiveSpecialEdInDistrict,"
                        "childrenReceiveSpecialEdNonDistrict,"
                        "hasMoreThan10Classes,"
                    );
                
                for ( j = 0; j < nara_pupils_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "\"pupils_%s_%s\",", nara_pupils_labels[j], nara_ethnicity_labels[i]);
                
                for ( j = 0; j < nara_special_ed_max; j++ )
                    for ( i = 0; i < nara_special_ed_category_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "\"specialEd_%s_%s\",", nara_special_ed_labels[j], nara_special_ed_category_labels[i]);
                
                for ( j = 0; j < nara_selected_course_max; j++ )
                    for ( i = 0; i < nara_selected_course_category_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "\"selectedCourse_%s_%s\",", nara_selected_course_labels[j], nara_selected_course_category_labels[i]);
                
                for ( j = 0; j < nara_ethnicity_max; j++ )
                    fprintf(CONTEXT->classroomFptr, "\"graduates_%s\",", nara_ethnicity_labels[j]);
                
                for ( j = 0; j < nara_classroom_survey_slots + nara_classroom_survey_slots_additional; j++ )
                    for ( i = 0; i < nara_classroom_survey_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "\"classroomSurvey_%u_%s\",", (j + 1), nara_classroom_survey_labels[i]);
                
                for ( i = 1; i <= 32; i++ )
                    fprintf(CONTEXT->classroomFptr, "errorBitArray_%d,", i);
                    
                for ( i = 1; i <= 82; i++ )
                    fprintf(CONTEXT->classroomFptr, "conditionCode_%d,", i);
                fprintf(CONTEXT->classroomFptr, "conditionCode_%d\n", i);
            }
            break;
        }
    }
}

/**/

void
__nara_export_destroy_summary(
    nara_export_context_t   exportContext
)
{
}

/**/

void
__nara_record_export_summary(
    nara_export_context_t   exportContext,
    nara_record_t           *theRecord
)
{
    nara_summary_t              *summary = (nara_summary_t*)theRecord;
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i, j, k;
    
    LOCAL_STR_DECL(systemName, sizeof(summary->systemName));
    LOCAL_STR_DECL(systemStreetAddress, sizeof(summary->systemStreetAddress));
    LOCAL_STR_DECL(systemCounty, sizeof(summary->systemCounty));
    LOCAL_STR_DECL(systemCity, sizeof(summary->systemCity));
    LOCAL_STR_DECL(systemStateAbbrev, sizeof(summary->systemStateAbbrev));
    LOCAL_STR_DECL(systemZipCode, sizeof(summary->systemZipCode));
    
    LOCAL_STR_FILL(systemName, sizeof(summary->systemName), summary->systemName);
    LOCAL_STR_FILL(systemStreetAddress, sizeof(summary->systemStreetAddress), summary->systemStreetAddress);
    LOCAL_STR_FILL(systemCounty, sizeof(summary->systemCounty), summary->systemCounty);
    LOCAL_STR_FILL(systemCity, sizeof(summary->systemCity), summary->systemCity);
    LOCAL_STR_FILL(systemStateAbbrev, sizeof(summary->systemStateAbbrev), summary->systemStateAbbrev);
    LOCAL_STR_FILL(systemZipCode, sizeof(summary->systemZipCode), summary->systemZipCode);
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
            
            if ( CONTEXT->fptr ) {
                fprintf(CONTEXT->fptr,
                        "- recordType: summary\n"
                        "  systemOECode: %u\n"
                        "  selectionCode: %u\n"
                        "  systemName: %s\n"
                        "  systemStreetAddress: %s\n"
                        "  systemCounty: %s\n"
                        "  systemCity: %s\n"
                        "  systemStatAbbrev: %s\n"
                        "  systemZipCode: %s\n"
                        "  numSchoolsInSchoolSystem: %u\n"
                        "  numSchoolsReporting: %u\n"
                        "  samplingWeight: %g\n"
                        "  isSubSampledDistrict: %u\n"
                        "  subSampledWeight: %g\n"
                        "  isSubSampledSchool: %u\n"
                        "  schoolShouldHaveCompletedPart6: %u\n"
                        "  numberOfClassroomsSurveyed: %u\n"
                        "  isSpecialEdProgramNotOffered: %u\n"
                        "  isItem7Completed: %u\n"
                        "  isSection3Completed: %u\n"
                        "  shouldSchoolHaveCompletedItem8: %u\n"
                        "  shouldSchoolHaveCompletedItem9: %u\n"
                        "  isCourtOrderYesFederal: %u\n"
                        "  isCourtOrderYesState: %u\n"
                        "  isCourtOrderNo: %u\n"
                        "  childrenAwaitingInitEval: %u\n"
                        "  childrenRequireSpecialEd: %u\n"
                        "  childrenReceiveSpecialEdInDistrict: %u\n"
                        "  childrenReceiveSpecialEdNonDistrict: %u\n"
                        "  hasMoreThan10Classes: %u\n",
                        summary->systemOECode,
                        summary->selectionCode,
                        systemName,
                        systemStreetAddress,
                        systemCounty,
                        systemCity,
                        systemStateAbbrev,
                        systemZipCode,
                        summary->numSchoolsInSchoolSystem,
                        summary->numSchoolsReporting,
                        summary->sampleWeight,
                        summary->isSubSampledDistrict,
                        summary->subSampledWeight,
                        summary->isSubSampledSchool,
                        summary->shouldSchoolHaveCompletedPart6,
                        summary->numberOfClassroomsSurveyed,
                        summary->isSpecialEdProgramNotOffered,
                        summary->isItem7Completed,
                        summary->isSection3Completed,
                        summary->shouldSchoolHaveCompletedItem8,
                        summary->shouldSchoolHaveCompletedItem9,
                        summary->isCourtOrderYesFederal,
                        summary->isCourtOrderYesState,
                        summary->isCourtOrderNo,
                        summary->childrenAwaitingInitEval,
                        summary->childrenRequireSpecialEd,
                        summary->childrenReceiveSpecialEdInDistrict,
                        summary->childrenReceiveSpecialEdNonDistrict,
                        summary->hasMoreThan10Classes
                    );
                
                if ( summary->numberOfClassroomsSurveyed > 0 ) {
                    fprintf(CONTEXT->fptr, "  classroomSurvey:\n");
                    k = (summary->numberOfClassroomsSurveyed < nara_classroom_survey_slots) ? summary->numberOfClassroomsSurveyed : nara_classroom_survey_slots;
                    for ( j = 0; j < k; j++ ) {
                        char        *prefix = "    - ";
                        for ( i = 0; i < nara_classroom_survey_max; i++ ) {
                            fprintf(CONTEXT->fptr, "%s\"%s\": %u\n", prefix, nara_classroom_survey_labels[i], summary->classroomSurveys[j][i]);
                            prefix = "      ";
                        }
                    }
                    k = summary->numberOfClassroomsSurveyed - k;
                    if ( k > 0 ) {
                        k = (k > nara_classroom_survey_slots_additional) ? nara_classroom_survey_slots_additional : k;
                        for ( j = 0; j < k; j++ ) {
                            char        *prefix = "    - ";
                            for ( i = 0; i < nara_classroom_survey_max; i++ ) {
                                fprintf(CONTEXT->fptr, "%s\"%s\": %u\n", prefix, nara_classroom_survey_labels[i], summary->additionalClassroomSurveys[j][i]);
                                prefix = "      ";
                            }
                        }
                    }
                }
                
                fprintf(CONTEXT->fptr, "  pupils:\n");
                for ( j = 0; j < nara_pupils_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_pupils_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], summary->pupilCounts[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  specialEd:\n");
                for ( j = 0; j < nara_special_ed_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_special_ed_labels[j]);
                    for ( i = 0; i < nara_special_ed_category_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_special_ed_category_labels[i], summary->specialEd[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  selectedCourses:\n");
                for ( j = 0; j < nara_selected_course_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_selected_course_labels[j]);
                    for ( i = 0; i < nara_selected_course_category_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_selected_course_category_labels[i], summary->selectedCourses[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  graduates:\n");
                for ( j = 0; j < nara_ethnicity_max; j++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[j], summary->graduateCounts[j]);

                fprintf(CONTEXT->fptr, "  errorBitArray:\n");
                for ( i = 0; i < 32; i++ )
                    fprintf(CONTEXT->fptr, "    - %u\n", summary->errorBitArray[i]);

                fprintf(CONTEXT->fptr, "  conditionCodes:\n");
                for ( i = 0; i < 82; i++ )
                    fprintf(CONTEXT->fptr, "    - %u\n", summary->conditionCodes[i]);
            }            
            break;
        }
        
        case nara_export_format_csv: {
            nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
            
            if ( CONTEXT->classroomFptr ) {
                fprintf(CONTEXT->classroomFptr,
                        "%u,"
                        "%u,"
                        "\"%s\","
                        "\"%s\","
                        "\"%s\","
                        "\"%s\","
                        "\"%s\","
                        "\"%s\","
                        "%u,"
                        "%u,"
                        "%g,"
                        "%u,"
                        "%g,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,",
                        summary->systemOECode,
                        summary->selectionCode,
                        systemName,
                        systemStreetAddress,
                        systemCounty,
                        systemCity,
                        systemStateAbbrev,
                        systemZipCode,
                        summary->numSchoolsInSchoolSystem,
                        summary->numSchoolsReporting,
                        summary->sampleWeight,
                        summary->isSubSampledDistrict,
                        summary->subSampledWeight,
                        summary->isSubSampledSchool,
                        summary->shouldSchoolHaveCompletedPart6,
                        summary->numberOfClassroomsSurveyed,
                        summary->isSpecialEdProgramNotOffered,
                        summary->isItem7Completed,
                        summary->isSection3Completed,
                        summary->shouldSchoolHaveCompletedItem8,
                        summary->shouldSchoolHaveCompletedItem9,
                        summary->isCourtOrderYesFederal,
                        summary->isCourtOrderYesState,
                        summary->isCourtOrderNo,
                        summary->childrenAwaitingInitEval,
                        summary->childrenRequireSpecialEd,
                        summary->childrenReceiveSpecialEdInDistrict,
                        summary->childrenReceiveSpecialEdNonDistrict,
                        summary->hasMoreThan10Classes
                    );
                
                for ( j = 0; j < nara_pupils_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "%u,", summary->pupilCounts[j][i]);
                
                for ( j = 0; j < nara_special_ed_max; j++ )
                    for ( i = 0; i < nara_special_ed_category_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "%u,", summary->specialEd[j][i]);
                
                for ( j = 0; j < nara_selected_course_max; j++ )
                    for ( i = 0; i < nara_selected_course_category_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "%u,", summary->selectedCourses[j][i]);
                
                for ( j = 0; j < nara_ethnicity_max; j++ )
                    fprintf(CONTEXT->classroomFptr, "%u,", summary->graduateCounts[j]);
                
                for ( j = 0; j < nara_classroom_survey_slots; j++ )
                    for ( i = 0; i < nara_classroom_survey_max; i++ )
                        fprintf(CONTEXT->classroomFptr, "%u,", summary->classroomSurveys[j][i]);

                for ( i = 0; i < 32; i++ )
                    fprintf(CONTEXT->classroomFptr, "%u,", summary->errorBitArray[i]);
                    
                for ( i = 0; i < 82; i++ )
                    fprintf(CONTEXT->classroomFptr, "%u,", summary->conditionCodes[i]);
                fprintf(CONTEXT->classroomFptr, "%u\n", summary->conditionCodes[i]);
            }
            break;
        }
    }
}

/**/

nara_record_t*
__nara_record_destroy_summary(
    nara_record_t       *theRecord
)
{
    free((void*)theRecord);
    return NULL;
}
    