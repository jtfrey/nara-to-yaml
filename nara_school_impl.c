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
    for ( i = 1; i < iMax; i ++ ) school->compact.f1[i] = nara_be_to_host_32(school->compact.f1[i]);
    
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
__nara_record_to_yaml_school(
    nara_record_t       *theRecord,
    FILE                *fptr
)
{
    nara_school_t    *school = (nara_school_t*)theRecord;
    unsigned int        i, j, iMax;
    
    LOCAL_STR_DECL(schoolName, sizeof(school->schoolName));
    LOCAL_STR_DECL(schoolStreetAddr, sizeof(school->schoolStreetAddr));
    LOCAL_STR_DECL(schoolCity, sizeof(school->schoolCity));
    LOCAL_STR_DECL(schoolCounty, sizeof(school->schoolCounty));
    
    LOCAL_STR_FILL(schoolName, sizeof(school->schoolName), school->schoolName);
    LOCAL_STR_FILL(schoolStreetAddr, sizeof(school->schoolStreetAddr), school->schoolStreetAddr);
    LOCAL_STR_FILL(schoolCity, sizeof(school->schoolCity), school->schoolCity);
    LOCAL_STR_FILL(schoolCounty, sizeof(school->schoolCounty), school->schoolCounty);
    
    fprintf(fptr,
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
        
    fprintf(fptr, "  gradeOffered:\n");
    for ( i = 0; i < nara_grade_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->gradeOffered[i]);
        
    fprintf(fptr, "  pupilCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilCounts[i]);
        
    fprintf(fptr, "  retainedCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->retainedCounts[i]);
        
    fprintf(fptr, "  grade12Counts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade12Counts[i]);
        
    fprintf(fptr, "  specialEdCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->specialEdCounts[i]);
        
    fprintf(fptr, "  emplCounts:\n");
    for ( j = 0; j < nara_empl_max; j++ ) {
        fprintf(fptr, "    \"%s\":\n", nara_empl_labels[j]);
        for ( i = 0; i < nara_ethnicity_max; i++ )
            fprintf(fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->emplCounts[j][i]);
    }
        
    fprintf(fptr, "  grade3Counts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade3Counts[i]);
        
    fprintf(fptr, "  grade6Counts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade6Counts[i]);
        
    fprintf(fptr, "  grade9Counts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->grade9Counts[i]);
        
    fprintf(fptr, "  lowestGradePupilCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->lowestGradePupilCounts[i]);
        
    fprintf(fptr, "  newStaffCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->newStaffCounts[i]);
        
    fprintf(fptr, "  participantInLunchProgramCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->participantInLunchProgramCounts[i]);
        
    fprintf(fptr, "  eligibleForLunchProgramCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->eligibleForLunchProgramCounts[i]);
        
    fprintf(fptr, "  receivingLunchProgramCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->receivingLunchProgramCounts[i]);
        
    fprintf(fptr, "  elementaryTeacherCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->elementaryTeacherCounts[i]);
        
    fprintf(fptr, "  secondaryTeacherCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->secondaryTeacherCounts[i]);
        
    fprintf(fptr, "  otherTeacherCounts:\n");
    for ( i = 0; i < nara_ethnicity_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->otherTeacherCounts[i]);
        
    fprintf(fptr, "  numSectionsInLowestGradeCounts:\n");
    for ( i = 0; i < nara_section_distrib_max; i++ )
        fprintf(fptr, "    \"%s\": %u\n", nara_section_distrib_labels[i], school->numSectionsInLowestGradeCounts[i]);
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
    