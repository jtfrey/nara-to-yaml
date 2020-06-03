/*
 * nara_school
 *
 * The NARA "school" type record as a data structure.  This is the public
 * interface to the record; there's also in internal one in nara_school_impl.c
 * that's used to process byte-swapping more efficiently.
 *
 */

#ifndef __NARA_SCHOOL_H__
#define __NARA_SCHOOL_H__

#include "nara_district.h"

enum {
    nara_grade_pre_k = 0,
    nara_grade_kindergarten,
    nara_grade_first,
    nara_grade_second,
    nara_grade_third,
    nara_grade_fourth,
    nara_grade_fifth,
    nara_grade_sixth,
    nara_grade_seventh,
    nara_grade_eighth,
    nara_grade_ninth,
    nara_grade_tenth,
    nara_grade_eleventh,
    nara_grade_twelfth,
    nara_grade_ungraded,
    nara_grade_special_ed,
    nara_grade_max
};

extern const char* nara_grade_labels[nara_grade_max];
    
enum {
    nara_empl_teacher = 0,
    nara_empl_principal,
    nara_empl_asst_principal,
    nara_empl_other,
    nara_empl_max
};

extern const char* nara_empl_labels[nara_empl_max];

enum {
    nara_section_distrib_0_to_19 = 0,
    nara_section_distrib_20_to_49,
    nara_section_distrib_50_to_79,
    nara_section_distrib_80_to_100,
    nara_section_distrib_max
};

extern const char* nara_section_distrib_labels[nara_section_distrib_max];

typedef struct {
    uint32_t        recordType;
    uint32_t        schoolSystemCode;
    uint32_t        schoolCampusFormNumber;
    char            schoolName[32];
    uint32_t        gradeOffered[nara_grade_max];
    uint32_t        pupilCounts[nara_ethnicity_max];
    uint32_t        retainedCounts[nara_ethnicity_max];
    uint32_t        grade12Counts[nara_ethnicity_max];
    uint32_t        specialEdCounts[nara_ethnicity_max];
    uint32_t        emplCounts[nara_empl_max][nara_ethnicity_max];
    char            filler[32];
    uint32_t        pupilsBused;
    uint32_t        departmentalizedEnglish;
    char            schoolStreetAddr[32];
    char            schoolCity[16];
    char            schoolCounty[32];
    uint32_t        schoolZipCode;
    uint32_t        grade3Counts[nara_ethnicity_max];
    uint32_t        grade6Counts[nara_ethnicity_max];
    uint32_t        grade9Counts[nara_ethnicity_max];
    uint32_t        lowestGradeOffered;
    uint32_t        lowestGradePupilCounts[nara_ethnicity_max];
    uint32_t        newStaffCounts[nara_ethnicity_max];
    uint32_t        lunchProgramOffered;
    uint32_t        participantInLunchProgramCounts[nara_ethnicity_max];
    uint32_t        eligibleForLunchProgramCounts[nara_ethnicity_max];
    uint32_t        receivingLunchProgramCounts[nara_ethnicity_max];
    uint32_t        elementaryTeacherCounts[nara_ethnicity_max];
    uint32_t        secondaryTeacherCounts[nara_ethnicity_max];
    uint32_t        otherTeacherCounts[nara_ethnicity_max];
    uint32_t        numSectionsInLowestGradeCounts[nara_section_distrib_max];
    uint32_t        oeCode1970;
} nara_school_t;

#endif /* __NARA_SCHOOL_H__ */
