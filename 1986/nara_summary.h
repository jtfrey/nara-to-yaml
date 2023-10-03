/*
 * nara_summary
 *
 * The NARA "summary" type record as a data structure.  This is the public
 * interface to the record; there's also in internal one in nara_summary_impl.c
 * that's used to process byte-swapping more efficiently.
 *
 */

#ifndef __NARA_SUMMARY_H__
#define __NARA_SUMMARY_H__


/* Alias the classroom type to summary: */
#define __nara_record_is_type_classroom __nara_record_is_type_summary
#define __nara_record_process_classroom __nara_record_process_summary
#define __nara_export_init_classroom __nara_export_init_summary
#define __nara_record_export_classroom __nara_record_export_summary
#define __nara_export_destroy_classroom __nara_export_destroy_summary
#define __nara_record_destroy_classroom __nara_record_destroy_summary


#include "nara_district.h"
#include "nara_school.h"

enum {
    nara_classroom_survey_slots_additional = 20
};

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
    uint32_t        numSchoolsReporting;
    float           sampleWeight;
    uint32_t        isSubSampledDistrict;
    float           subSampledWeight;
    uint32_t        isSubSampledSchool;
    
    uint32_t        pupilCounts[nara_pupils_max][nara_ethnicity_max];
    uint32_t        shouldSchoolHaveCompletedPart6;
    uint32_t        numberOfClassroomsSurveyed;
    uint32_t        classroomSurveys[nara_classroom_survey_slots][nara_classroom_survey_max];
    uint32_t        isSpecialEdProgramNotOffered;
    uint32_t        isItem7Completed;
    uint32_t        specialEd[nara_special_ed_max][nara_special_ed_category_max];
    uint32_t        isSection3Completed;
    uint32_t        shouldSchoolHaveCompletedItem8;
    uint32_t        selectedCourses[nara_selected_course_max][nara_selected_course_category_max];
    uint32_t        shouldSchoolHaveCompletedItem9;
    uint32_t        graduateCounts[nara_ethnicity_max];
    
    uint32_t        isCourtOrderYesFederal;
    uint32_t        isCourtOrderYesState;
    uint32_t        isCourtOrderNo;
    uint32_t        childrenAwaitingInitEval;
    uint32_t        childrenRequireSpecialEd;
    uint32_t        childrenReceiveSpecialEdInDistrict;
    uint32_t        childrenReceiveSpecialEdNonDistrict;
    
    uint32_t        unused1[5];
    
    uint32_t        hasMoreThan10Classes;
    uint32_t        additionalClassroomSurveys[nara_classroom_survey_slots_additional][nara_classroom_survey_max];
    
    uint32_t        unused2[8];
    
    uint32_t        errorBitArray[32];
    uint32_t        conditionCodes[82];
    
    uint32_t        unused3[24];
} nara_summary_t;

#endif /* __NARA_SUMMARY_H__ */
