/*
 * nara_district
 *
 * The NARA "district" type record as a data structure.  This is the public
 * interface to the record; there's also in internal one in nara_district_impl.c
 * that's used to process byte-swapping more efficiently.
 *
 */

#ifndef __NARA_DISTRICT_H__
#define __NARA_DISTRICT_H__

#include "nara_base.h"

enum {
    nara_ethnicity_american_alaskan_indian = 0,
    nara_ethnicity_asian_pacific,
    nara_ethnicity_black_not_hispanic,
    nara_ethnicity_white_not_hispanic,
    nara_ethnicity_hispanic,
    nara_ethnicity_total,
    nara_ethnicity_max
};

extern const char* nara_ethnicity_labels[nara_ethnicity_max];

enum {
    nara_gender_male = 0,
    nara_gender_female,
    nara_gender_max
};

extern const char* nara_gender_labels[nara_gender_max];


typedef struct {
    uint32_t        systemOECode;
    uint32_t        recordType;
    uint32_t        selectionCode;
    char            systemName[32];
    char            systemCounty[16];
    char            systemCity[16];
    char            systemZipCode[8];
    uint32_t        numSchoolsInSchoolSystem;
    uint32_t        isInConsolidation;
    uint32_t        isInUnification;
    uint32_t        isInDivision;
    uint32_t        isInAnnexation;
    uint32_t        isNotInAnyStateOfChange;
    uint32_t        isUnderCourtOrderToDesegregate;
    uint32_t        doGenderGradRequirementsDiffer;
    uint32_t        pupils[nara_gender_max][nara_ethnicity_max];
    uint32_t        pupilsEnrolledVocationEd[nara_gender_max][nara_ethnicity_max];
    uint32_t        numSchoolsWith5OrMoreVocationEdPrograms;
    uint32_t        pupilsSuspendedAtLeastOneDay[nara_gender_max][nara_ethnicity_max];
    uint32_t        pupilsSuspendedAtLeastOneDayTotal[nara_ethnicity_max];
    uint32_t        pupilsPrimaryLangNotEnglishTotal[nara_ethnicity_max];
    uint32_t        pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal[nara_ethnicity_max];
    uint32_t        pupilsSpecialEdTotal[nara_ethnicity_max];
    uint32_t        pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal[nara_ethnicity_max];
    uint32_t        pupilsSpecialEdForGiftedOrTalentedTotal[nara_ethnicity_max];
    uint32_t        pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal[nara_ethnicity_max];
    uint32_t        residentSchoolAgeChildrenIdentifiedRequiringSpecialEd;
    uint32_t        residentPupilsInSpecialEdOperatedWithOtherSchoolSystems;
    uint32_t        residentPupilsInSpecialEdOperatedExclOtherSchoolSystem;
    uint32_t        residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem;
    uint32_t        nonResidentPupilsInSpecialEd;
    uint32_t        residentSchoolAgeChildrenOutOfSchoolHandicappingCondition;
    uint32_t        residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction;
    uint32_t        residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds;
    uint32_t        fullTimeTeachersAssignedToSpecialEd;
    uint32_t        partTimeTeachersAssignedToSpecialEd;
    uint32_t        hasOtherReportingDates;
    uint32_t        unused1;
    uint32_t        unused2;
    uint32_t        isESAADistrict;
    float           samplingWeight;
    uint32_t        unused3[706];
    uint32_t        errorBitArray[43];
} nara_district_t;

#endif /* __NARA_DISTRICT_H__ */
