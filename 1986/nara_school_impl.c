/*
 * nara_school_impl
 *
 * The NARA "school" record implementation.
 *
 */

#include "nara_school.h"

const char* nara_ethnicity_labels[nara_ethnicity_max] = {
            "American Indian",
            "Asian or Pacific",
            "Hispanic",
            "Black",
            "White",
            "Total",
            "Total (Male)",
            "Total (Female)"
        };

const char* nara_grade_labels[nara_grade_max] = {
            "Ungraded",
            "Only Special-Ed",
            "Pre-K",
            "Kindergarten",
            "1st",
            "2nd",
            "3rd",
            "4th",
            "5th",
            "6th",
            "7th",
            "8th",
            "9th",
            "10th",
            "11th",
            "12th"
        };

const char* nara_pupils_labels[nara_pupils_max] = {
            "Total Pupils",
            "Pupils Needing Language Assistance",
            "Pupils Enrolled Language Assistance",
            "Pupils in Gifted Program",
            "Pupils Receiving Corporal Punishment",
            "Pupils Suspended"
        };

const char* nara_classroom_survey_labels[nara_classroom_survey_max] = {
            "Grade Level",
            "American Indian",
            "Asian or Pacific",
            "Hispanic",
            "Black",
            "White",
            "Total"
        };

const char* nara_special_ed_category_labels[nara_special_ed_category_max] = {
            "Total",
            "American Indian",
            "Asian or Pacific",
            "Hispanic",
            "Black",
            "White",
            "Total (Male)",
            "Total (Female)",
            "Limited or Non-English Speaking",
            "Part-time",
            "Full-time"
        };


const char* nara_special_ed_labels[nara_special_ed_max] = {
            "Educable Mentally Retarded",
            "Trainable Mentally Retarded",
            "Hard of Hearing",
            "Deaf",
            "Speech Impairment",
            "Blind or Visually Impaired",
            "Seriously Emotionally Disturbed",
            "Orthopedically Impaired",
            "Other Health Impairment",
            "Specific Learning Disability",
            "Deaf and Blind",
            "Multiple Handicaps",
            "Total of All Impairments"
        };

const char* nara_selected_course_category_labels[nara_selected_course_category_max] = {
            "Non-mixed (Male)",
            "Non-mixed (Female)",
            "Mixed (Male)",
            "Mixed (Female)",
            "Total"
        };

const char* nara_selected_course_labels[nara_selected_course_max] = {
            "Home Economics",
            "Industrial Arts",
            "Physical Education"
        };


typedef union {
    nara_school_t    break_out;
    struct {
        uint32_t        f1[3];
        char            schoolName[32];
        char            schoolStreetAddress[28];
        char            schoolZipCode[8];
        uint32_t        f2[nara_grade_max + nara_pupils_max * nara_ethnicity_max + 2 + nara_classroom_survey_slots * nara_classroom_survey_max + 2 + nara_special_ed_max * nara_special_ed_category_max + 2 + nara_selected_course_max * nara_selected_course_category_max + 1 + nara_ethnicity_max];
        uint32_t        unused1[3];
        float           f3;
        uint32_t        f4;
        float           f5;
        uint32_t        f6;
        uint32_t        unused2[5];
    } compact;
} nara_school_internal_t;

/**/

int
__nara_record_is_type_school(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == NARA_1986_RECORD_SIZE) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_school) );
}

/**/

nara_record_t*
__nara_record_process_school(
    nara_record_t*      theRecord
)
{
    nara_school_internal_t      *school = (nara_school_internal_t*)theRecord;
    char                        convBuffer[64];
    char                        *fromPtr, *toPtr;
    size_t                      fromLen, toLen;
    
    unsigned int                i, iMax;
    
    iMax = sizeof(school->compact.f1) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) school->compact.f1[i] = nara_be_to_host_32(school->compact.f1[i]);
    
    iMax = sizeof(school->compact.f2) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) school->compact.f2[i] = nara_be_to_host_32(school->compact.f2[i]);
    
    school->compact.f3 = nara_be_to_host_float(school->compact.f3);
    
    school->compact.f4 = nara_be_to_host_32(school->compact.f4);
    
    school->compact.f5 = nara_be_to_host_float(school->compact.f5);
    
    school->compact.f6 = nara_be_to_host_32(school->compact.f6);
    
    TRANSCODE(school, schoolName, "school name");
    TRANSCODE(school, schoolStreetAddress, "school street address");
    TRANSCODE(school, schoolZipCode, "school zip code");
    
    return theRecord;
}

/**/

void
__nara_export_init_school(
    nara_export_context_t   exportContext
)
{
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i, j, k, l, m, idx;
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            break;
        }
        
        case nara_export_format_csv: {
            nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
            
            /*
             * Write column headers to the file:
             */
            if ( CONTEXT->schoolFptr ) {
                fprintf(CONTEXT->schoolFptr,
                        "systemOECode,"
                        "selectionCode,"
                        "schoolName,"
                        "schoolStreetAddress,"
                        "schoolZipCode,"
                        "schoolShouldHaveCompletedPart6,"
                        "isSpecialEdProgramNotOffered,"
                        "isItem7Completed,"
                        "isSection3Completed,"
                        "shouldSchoolHaveCompletedItem8,"
                        "shouldSchoolHaveCompletedItem9,"
                        "samplingWeight,"
                        "isSubSampledDistrict,"
                        "subSampledWeight,"
                        "isSubSampledSchool,"
                        "numberOfClassroomsSurveyed,"
                    );
                
                for ( j = 0; j < nara_classroom_survey_slots; j++ )
                    for ( i = 0; i < nara_classroom_survey_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"classroomSurvey_%u_%s\",", (j + 1), nara_classroom_survey_labels[i]);
                
                for ( j = 0; j < nara_grade_max; j++ )
                    fprintf(CONTEXT->schoolFptr, "\"isGradeOffered%s\",", nara_grade_labels[j]);
                
                for ( j = 0; j < nara_pupils_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupils_%s_%s\",", nara_pupils_labels[j], nara_ethnicity_labels[i]);
                
                for ( j = 0; j < nara_special_ed_max; j++ )
                    for ( i = 0; i < nara_special_ed_category_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"specialEd_%s_%s\",", nara_special_ed_labels[j], nara_special_ed_category_labels[i]);
                
                for ( j = 0; j < nara_selected_course_max; j++ )
                    for ( i = 0; i < nara_selected_course_category_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"selectedCourse_%s_%s\",", nara_selected_course_labels[j], nara_selected_course_category_labels[i]);
                
                for ( j = 0; j < nara_ethnicity_max - 1; j++ )
                    fprintf(CONTEXT->schoolFptr, "\"graduates_%s\",", nara_ethnicity_labels[j]);
                fprintf(CONTEXT->schoolFptr, "\"graduates_%s\"\n", nara_ethnicity_labels[j]);
            }
            break;
        }
    }
}

/**/

void
__nara_export_destroy_school(
    nara_export_context_t   exportContext
)
{
}

/**/

void
__nara_record_export_school(
    nara_export_context_t   exportContext,
    nara_record_t           *theRecord
)
{
    nara_school_t               *school = (nara_school_t*)theRecord;
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i, j, k, l, m;
    
    LOCAL_STR_DECL(schoolName, sizeof(school->schoolName));
    LOCAL_STR_DECL(schoolStreetAddress, sizeof(school->schoolStreetAddress));
    LOCAL_STR_DECL(schoolZipCode, sizeof(school->schoolZipCode));
    
    LOCAL_STR_FILL(schoolName, sizeof(school->schoolName), school->schoolName);
    LOCAL_STR_FILL(schoolStreetAddress, sizeof(school->schoolStreetAddress), school->schoolStreetAddress);
    LOCAL_STR_FILL(schoolZipCode, sizeof(school->schoolZipCode), school->schoolZipCode);
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
            
            if ( CONTEXT->fptr ) {
                fprintf(CONTEXT->fptr,
                        "- recordType: school\n"
                        "  systemOECode: %u\n"
                        "  selectionCode: %u\n"
                        "  schoolName: %s\n"
                        "  schoolStreetAddress: %s\n"
                        "  schoolZipCode: %s\n"
                        "  schoolShouldHaveCompletedPart6: %u\n"
                        "  isSpecialEdProgramNotOffered: %u\n"
                        "  isItem7Completed: %u\n"
                        "  isSection3Completed: %u\n"
                        "  shouldSchoolHaveCompletedItem8: %u\n"
                        "  shouldSchoolHaveCompletedItem9: %u\n"
                        "  samplingWeight: %g\n"
                        "  isSubSampledDistrict: %u\n"
                        "  subSampledWeight: %g\n"
                        "  isSubSampledSchool: %u\n"
                        "  numberOfClassroomsSurveyed: %u\n",
                        school->systemOECode,
                        school->selectionCode,
                        schoolName,
                        schoolStreetAddress,
                        schoolZipCode,
                        school->shouldSchoolHaveCompletedPart6,
                        school->isSpecialEdProgramNotOffered,
                        school->isItem7Completed,
                        school->isSection3Completed,
                        school->shouldSchoolHaveCompletedItem8,
                        school->shouldSchoolHaveCompletedItem9,
                        school->sampleWeight,
                        school->isSubSampledDistrict,
                        school->subSampledWeight,
                        school->isSubSampledSchool,
                        school->numberOfClassroomsSurveyed
                    );
                
                if ( school->numberOfClassroomsSurveyed > 0 ) {
                    fprintf(CONTEXT->fptr, "  classroomSurvey:\n");
                    k = (school->numberOfClassroomsSurveyed < nara_classroom_survey_slots) ? school->numberOfClassroomsSurveyed : nara_classroom_survey_slots;
                    for ( j = 0; j < k; j++ ) {
                        char        *prefix = "    - ";
                        for ( i = 0; i < nara_classroom_survey_max; i++ ) {
                            fprintf(CONTEXT->fptr, "%s\"%s\": %u\n", prefix, nara_classroom_survey_labels[i], school->classroomSurveys[j][i]);
                            prefix = "      ";
                        }
                    }
                }
                
                fprintf(CONTEXT->fptr, "  isGradeOffered:\n");
                for ( j = 0; j < nara_grade_max; j++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[j], school->isGradeOffered[j]);
                
                fprintf(CONTEXT->fptr, "  pupils:\n");
                for ( j = 0; j < nara_pupils_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_pupils_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilCounts[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  specialEd:\n");
                for ( j = 0; j < nara_special_ed_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_special_ed_labels[j]);
                    for ( i = 0; i < nara_special_ed_category_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_special_ed_category_labels[i], school->specialEd[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  selectedCourses:\n");
                for ( j = 0; j < nara_selected_course_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_selected_course_labels[j]);
                    for ( i = 0; i < nara_selected_course_category_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_selected_course_category_labels[i], school->selectedCourses[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  graduates:\n");
                for ( j = 0; j < nara_ethnicity_max; j++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[j], school->graduateCounts[j]);
            }            
            break;
        }
        
        case nara_export_format_csv: {
            nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
            
            if ( CONTEXT->schoolFptr ) {
                fprintf(CONTEXT->schoolFptr,
                        "%u,"
                        "%u,"
                        "\"%s\","
                        "\"%s\","
                        "\"%s\","
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%g,"
                        "%u,"
                        "%g,"
                        "%u,"
                        "%u,",
                        school->systemOECode,
                        school->selectionCode,
                        schoolName,
                        schoolStreetAddress,
                        schoolZipCode,
                        school->shouldSchoolHaveCompletedPart6,
                        school->isSpecialEdProgramNotOffered,
                        school->isItem7Completed,
                        school->isSection3Completed,
                        school->shouldSchoolHaveCompletedItem8,
                        school->shouldSchoolHaveCompletedItem9,
                        school->sampleWeight,
                        school->isSubSampledDistrict,
                        school->subSampledWeight,
                        school->isSubSampledSchool,
                        school->numberOfClassroomsSurveyed
                    );
                
                for ( j = 0; j < nara_classroom_survey_slots; j++ )
                    for ( i = 0; i < nara_classroom_survey_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->classroomSurveys[j][i]);
                
                for ( j = 0; j < nara_grade_max; j++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->isGradeOffered[j]);
                
                for ( j = 0; j < nara_pupils_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilCounts[j][i]);
                
                for ( j = 0; j < nara_special_ed_max; j++ )
                    for ( i = 0; i < nara_special_ed_category_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->specialEd[j][i]);
                
                for ( j = 0; j < nara_selected_course_max; j++ )
                    for ( i = 0; i < nara_selected_course_category_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->selectedCourses[j][i]);
                
                for ( j = 0; j < nara_ethnicity_max - 1; j++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->graduateCounts[j]);
                fprintf(CONTEXT->schoolFptr, "%u\n", school->graduateCounts[j]);
            }
            break;
        }
        
    }
}

/**/

nara_record_t*
__nara_record_destroy_school(
    nara_record_t       *theRecord
)
{
    free((void*)theRecord);
    return NULL;
}
