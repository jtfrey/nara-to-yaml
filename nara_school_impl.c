/*
 * nara_school_impl
 *
 * The NARA "school" record implementation.
 *
 */

#include "nara_school.h"

const char* nara_grade_labels[nara_grade_max] = {
        "Pre-K",
        "Kindergarten",
        "First",
        "Second",
        "Third",
        "Fourth",
        "Fifth",
        "Sixth",
        "Seventh",
        "Eighth",
        "Ninth",
        "Tenth",
        "Eleventh",
        "Twelfth",
        "Ungraded",
        "Special Education"
    };

const char* nara_empl_labels[nara_empl_max] = {
        "Teacher",
        "Principal",
        "Assistant Principal",
        "Other"
    };

const char* nara_section_distrib_labels[nara_section_distrib_max] = {
        "0% to 19%",
        "20% to 49%",
        "50% to 79%",
        "80% to 100%"
    };

typedef union {
    nara_school_t    break_out;
    struct {
        uint32_t        f1[3];
        char            schoolName[32];
        uint32_t        f2[nara_grade_max+ 4 * nara_ethnicity_max + nara_empl_max * nara_ethnicity_max];
        char            filler[32];
        uint32_t        f3[2];
        char            schoolStreetAddr[32];
        char            schoolCity[16];
        char            schoolCounty[32];
        uint32_t        f4[1 + 3 * nara_ethnicity_max + 1 + 2 * nara_ethnicity_max + 1 + 6 * nara_ethnicity_max + nara_section_distrib_max + 1];
    } compact;
} nara_school_internal_t;

/**/

int
__nara_record_is_type_school(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == 716) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_school) );
}

/**/

nara_record_t*
__nara_record_process_school(
    nara_record_t*      theRecord,
    iconv_t             encodingConverter
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
    
    iMax = sizeof(school->compact.f3) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) school->compact.f3[i] = nara_be_to_host_32(school->compact.f3[i]);
    
    iMax = sizeof(school->compact.f4) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) school->compact.f4[i] = nara_be_to_host_32(school->compact.f4[i]);
    
    iconv(encodingConverter, NULL, NULL, NULL, NULL);
    TRANSCODE(school, schoolName, "school name");
    TRANSCODE(school, schoolStreetAddr, "school street address");
    TRANSCODE(school, schoolCity, "school city");
    TRANSCODE(school, schoolCounty, "school county");
    
    return theRecord;
}

/**/

void
__nara_export_init_school(
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
            if ( CONTEXT->schoolFptr ) {
                fprintf(CONTEXT->schoolFptr,
                        "recordType,"
                        "schoolSystemCode,"
                        "oeCode1970,"
                        "schoolCampusFormNumber,"
                        "pupilsBused,"
                        "departmentalizedEnglish,"
                        "schoolName,"
                        "schoolStreetAddr,"
                        "schoolCity,"
                        "schoolCounty,"
                        "schoolZipCode,"
                        "lowestGradeOffered,"
                        "lunchProgramOffered,"
                    );
                    
                for ( i = 0; i < nara_grade_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "gradeOffered_%s,", nara_grade_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "pupilCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "retainedCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "grade12Counts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "specialEdCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( j = 0; j < nara_empl_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "emplCounts_%s_%s,", nara_empl_labels[j], nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "grade3Counts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "grade6Counts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "grade9Counts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "lowestGradePupilCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "newStaffCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "participantInLunchProgramCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "eligibleForLunchProgramCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "receivingLunchProgramCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "elementaryTeacherCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "secondaryTeacherCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "otherTeacherCounts_%s,", nara_ethnicity_labels[i]);
        
                for ( i = 0; i < nara_section_distrib_max - 1; i++ )
                    fprintf(CONTEXT->schoolFptr, "numSectionsInLowestGradeCounts_%s,", nara_section_distrib_labels[i]);
                fprintf(CONTEXT->schoolFptr, "numSectionsInLowestGradeCounts_%s\n", nara_section_distrib_labels[i]);
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
    unsigned int                i, j;
    
    LOCAL_STR_DECL(schoolName, sizeof(school->schoolName));
    LOCAL_STR_DECL(schoolStreetAddr, sizeof(school->schoolStreetAddr));
    LOCAL_STR_DECL(schoolCity, sizeof(school->schoolCity));
    LOCAL_STR_DECL(schoolCounty, sizeof(school->schoolCounty));
    
    LOCAL_STR_FILL(schoolName, sizeof(school->schoolName), school->schoolName);
    LOCAL_STR_FILL(schoolStreetAddr, sizeof(school->schoolStreetAddr), school->schoolStreetAddr);
    LOCAL_STR_FILL(schoolCity, sizeof(school->schoolCity), school->schoolCity);
    LOCAL_STR_FILL(schoolCounty, sizeof(school->schoolCounty), school->schoolCounty);
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
            
            if ( CONTEXT->fptr ) {
                fprintf(CONTEXT->fptr,
                        "- recordType: school\n"
                        "  schoolSystemCode: %u\n"
                        "  oeCode1970: %u\n"
                        "  schoolCampusFormNumber: %u\n"
                        "  pupilsBused: %u\n"
                        "  departmentalizedEnglish: %u\n"
                        "  schoolName: \"%s\"\n"
                        "  schoolStreetAddr: \"%s\"\n"
                        "  schoolCity: \"%s\"\n"
                        "  schoolCounty: \"%s\"\n"
                        "  schoolZipCode: %u\n"
                        "  lowestGradeOffered: %u\n"
                        "  lunchProgramOffered: %u\n"
                        ,
                        school->schoolSystemCode,
                        school->oeCode1970,
                        school->schoolCampusFormNumber,
                        school->pupilsBused,
                        school->departmentalizedEnglish,
                        schoolName,
                        schoolStreetAddr,
                        schoolCity,
                        schoolCounty,
                        school->schoolZipCode,
                        school->lowestGradeOffered,
                        school->lunchProgramOffered
                    );
        
                fprintf(CONTEXT->fptr, "  gradeOffered:\n");
                for ( i = 0; i < nara_grade_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->gradeOffered[i]);
        
                fprintf(CONTEXT->fptr, "  pupilCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilCounts[i]);
        
                fprintf(CONTEXT->fptr, "  retainedCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->retainedCounts[i]);
        
                fprintf(CONTEXT->fptr, "  grade12Counts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade12Counts[i]);
        
                fprintf(CONTEXT->fptr, "  specialEdCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->specialEdCounts[i]);
        
                fprintf(CONTEXT->fptr, "  emplCounts:\n");
                for ( j = 0; j < nara_empl_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_empl_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->emplCounts[j][i]);
                }
        
                fprintf(CONTEXT->fptr, "  grade3Counts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade3Counts[i]);
        
                fprintf(CONTEXT->fptr, "  grade6Counts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade6Counts[i]);
        
                fprintf(CONTEXT->fptr, "  grade9Counts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade9Counts[i]);
        
                fprintf(CONTEXT->fptr, "  lowestGradePupilCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->lowestGradePupilCounts[i]);
        
                fprintf(CONTEXT->fptr, "  newStaffCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->newStaffCounts[i]);
        
                fprintf(CONTEXT->fptr, "  participantInLunchProgramCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->participantInLunchProgramCounts[i]);
        
                fprintf(CONTEXT->fptr, "  eligibleForLunchProgramCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->eligibleForLunchProgramCounts[i]);
        
                fprintf(CONTEXT->fptr, "  receivingLunchProgramCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->receivingLunchProgramCounts[i]);
        
                fprintf(CONTEXT->fptr, "  elementaryTeacherCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->elementaryTeacherCounts[i]);
        
                fprintf(CONTEXT->fptr, "  secondaryTeacherCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->secondaryTeacherCounts[i]);
        
                fprintf(CONTEXT->fptr, "  otherTeacherCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->otherTeacherCounts[i]);
        
                fprintf(CONTEXT->fptr, "  numSectionsInLowestGradeCounts:\n");
                for ( i = 0; i < nara_section_distrib_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_section_distrib_labels[i], school->numSectionsInLowestGradeCounts[i]);
            }
            break;
        }
        
        case nara_export_format_csv: {
            nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
            
            if ( CONTEXT->schoolFptr ) {
                fprintf(CONTEXT->schoolFptr,
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "\"%s\","
                        "\"%s\","
                        "\"%s\","
                        "\"%s\","
                        "%u,"
                        "%u,"
                        "%u,",
                        school->schoolSystemCode,
                        school->oeCode1970,
                        school->schoolCampusFormNumber,
                        school->pupilsBused,
                        school->departmentalizedEnglish,
                        schoolName,
                        schoolStreetAddr,
                        schoolCity,
                        schoolCounty,
                        school->schoolZipCode,
                        school->lowestGradeOffered,
                        school->lunchProgramOffered
                    );
                for ( i = 0; i < nara_grade_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->gradeOffered[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->retainedCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->grade12Counts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->specialEdCounts[i]);

                for ( j = 0; j < nara_empl_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->emplCounts[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->grade3Counts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->grade6Counts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->grade9Counts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->lowestGradePupilCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->newStaffCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->participantInLunchProgramCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->eligibleForLunchProgramCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->receivingLunchProgramCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->elementaryTeacherCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->secondaryTeacherCounts[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->otherTeacherCounts[i]);

                for ( i = 0; i < nara_section_distrib_max - 1; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->numSectionsInLowestGradeCounts[i]);
                fprintf(CONTEXT->schoolFptr, "%u\n", school->numSectionsInLowestGradeCounts[i]);
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
    