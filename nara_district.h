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
    nara_ethnicity_american_indian = 0,
    nara_ethnicity_black,
    nara_ethnicity_asian_american,
    nara_ethnicity_spanish_surname,
    nara_ethnicity_other,
    nara_ethnicity_total,
    nara_ethnicity_max
};

extern const char* nara_ethnicity_labels[nara_ethnicity_max];

typedef struct {
    uint32_t        recordType;
    uint32_t        schoolSystemCode;
    uint32_t        unused;
    char            systemName[32];
    char            systemStreetAddr[32];
    char            systemCity[16];
    char            systemCounty[16];
    char            systemState[16];
    char            systemZipCode[8];
    char            systemAdminOfficer[20];
    uint32_t        numSchoolCampusForms;
    uint32_t        pupilCounts[nara_ethnicity_max];
    uint32_t        nonResidentPupils;
    uint32_t        residentPupils;
    uint32_t        expelledPupilCounts[nara_ethnicity_max];
    uint32_t        residentPupilsInOtherSystem;
    uint32_t        residentPupilsInNonpublicSchools;
    uint32_t        residentSchoolAgeNotInSchool;
    uint32_t        systemTeacherCounts[nara_ethnicity_max];
    uint32_t        professionalStaffCounts[nara_ethnicity_max];
    uint32_t        professionalsInMoreThanOneSchool[nara_ethnicity_max];
    uint32_t        bilingualInstruction;
    uint32_t        bilingualTeacherCount;
    uint32_t        bilingualPupilCount;
    uint32_t        bilingualInstructionMaterials;
    uint32_t        newSchoolProperty;
    uint32_t        newSchoolConstruction;
    uint32_t        newSchoolCapacity;
    uint32_t        newSchoolGreaterMinorityComposition;
    uint32_t        year;
    uint32_t        stateCode;
    char            srgCode[16];
    uint32_t        assurance;
    uint32_t        litigationCode;
    uint32_t        selectionCode;
    uint32_t        samplingWeight;
    uint32_t        pupilsInAnotherSystemCounts[nara_ethnicity_total];
    uint32_t        pupilsInNonPublicSchoolsCounts[nara_ethnicity_total];
    uint32_t        pupilsSchoolAgeNotInSchoolCounts[nara_ethnicity_total];
    uint32_t        pupilsNonResidentCounts[nara_ethnicity_total];
    uint32_t        pupilsResidentCounts[nara_ethnicity_total];
    uint32_t        oeCode1970;
} nara_district_t;

#endif /* __NARA_DISTRICT_H__ */
