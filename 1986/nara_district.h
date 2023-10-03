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

typedef struct {
    uint32_t        systemOECode;
    uint32_t        recordType;
    uint32_t        selectionCode;
    char            systemName[32];
    char            systemStreetAddress[32];
    char            systemCounty[16];
    char            systemCity[16];
    char            systemStateAbbrev[4];
    char            systemZipCode[8];
    uint32_t        numSchoolsInSchoolSystem;
    uint32_t        isCourtOrderYesFederal;
    uint32_t        isCourtOrderYesState;
    uint32_t        isCourtOrderNo;
    uint32_t        childrenAwaitingInitEval;
    uint32_t        childrenRequireSpecialEd;
    uint32_t        childrenReceiveSpecialEdInDistrict;
    uint32_t        childrenReceiveSpecialEdNonDistrict;
    uint32_t        unused1[292];
    float           sampleWeight;
    uint32_t        isSubSampledDistrict;
    float           subSampledWeight;
    uint32_t        isSubSampledSchool;
    uint32_t        unused2[154];
    uint32_t        errorBitArray[32];
    uint32_t        conditionCodes[82];
    uint32_t        unused3[98];
} nara_district_t;

#endif /* __NARA_DISTRICT_H__ */
