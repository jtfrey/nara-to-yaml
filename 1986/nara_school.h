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
    nara_ethnicity_american_indian = 0,
    nara_ethnicity_asian_pacific,
    nara_ethnicity_hispanic,
    nara_ethnicity_black,
    nara_ethnicity_white,
    nara_ethnicity_total,
    nara_ethnicity_total_male,
    nara_ethnicity_total_female,
    nara_ethnicity_max
};

extern const char* nara_ethnicity_labels[nara_ethnicity_max];


enum {
    nara_grade_totally_ungraded = 0,
    nara_grade_only_special_ed,
    nara_grade_pre_kindergarten,
    nara_grade_kindergarten,
    nara_grade_1,
    nara_grade_2,
    nara_grade_3,
    nara_grade_4,
    nara_grade_5,
    nara_grade_6,
    nara_grade_7,
    nara_grade_8,
    nara_grade_9,
    nara_grade_10,
    nara_grade_11,
    nara_grade_12,
    nara_grade_max
};

extern const char* nara_grade_labels[nara_grade_max];


enum {
    nara_pupils_total = 0,
    nara_pupils_needing_language_assistance,
    nara_pupils_enrolled_language_assistance,
    nara_pupils_in_gifted_program,
    nara_pupils_receive_corporal_punishment,
    nara_pupils_suspended,
    nara_pupils_max
};

extern const char* nara_pupils_labels[nara_pupils_max];


enum {
    nara_classroom_survey_grade = 0,
    nara_classroom_survey_american_indian,
    nara_classroom_survey_asian_pacific,
    nara_classroom_survey_hispanic,
    nara_classroom_survey_black,
    nara_classroom_survey_white,
    nara_classroom_survey_total,
    nara_classroom_survey_max
};

extern const char* nara_classroom_survey_labels[nara_classroom_survey_max];

enum {
    nara_classroom_survey_slots = 10
};


enum {
    nara_special_ed_category_total = 0,
    nara_special_ed_category_american_indian,
    nara_special_ed_category_asian_pacific,
    nara_special_ed_category_hispanic,
    nara_special_ed_category_black,
    nara_special_ed_category_white,
    nara_special_ed_category_total_male,
    nara_special_ed_category_total_female,
    nara_special_ed_category_limited_or_nonenglish_speaking,
    nara_special_ed_category_part_time,
    nara_special_ed_category_full_time,
    nara_special_ed_category_max
};

extern const char* nara_special_ed_category_labels[nara_special_ed_category_max];


enum {
    nara_special_ed_educable_mentally_retarded = 0,
    nara_special_ed_trainable_metally_retarded,
    nara_special_ed_hard_of_hearing,
    nara_special_ed_deaf,
    nara_special_ed_speech_impaired,
    nara_special_ed_visually_handicapped,
    nara_special_ed_seriously_emotionally_disturbed,
    nara_special_ed_orthopedically_impaired,
    nara_special_ed_other_health_impaired,
    nara_special_ed_specific_learning_disability,
    nara_special_ed_deaf_blind,
    nara_special_ed_multihandicapped,
    nara_special_ed_totals,
    nara_special_ed_max
};

extern const char* nara_special_ed_labels[nara_special_ed_max];


enum {
    nara_selected_course_category_all_male = 0,
    nara_selected_course_category_all_female,
    nara_selected_course_category_mixed_male,
    nara_selected_course_category_mixed_female,
    nara_selected_course_category_total,
    nara_selected_course_category_max
};

extern const char* nara_selected_course_category_labels[nara_selected_course_category_max];


enum {
    nara_selected_course_home_economics = 0,
    nara_selected_course_industrial_arts,
    nara_selected_course_physical_education,
    nara_selected_course_max
};

extern const char* nara_selected_course_labels[nara_selected_course_max];


typedef struct {
    uint32_t        systemOECode;
    uint32_t        recordType;
    uint32_t        selectionCode;
    char            schoolName[32];
    char            schoolStreetAddress[28];
    char            schoolZipCode[8];
    uint32_t        isGradeOffered[nara_grade_max];
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
    uint32_t        unused1[3];
    float           sampleWeight;
    uint32_t        isSubSampledDistrict;
    float           subSampledWeight;
    uint32_t        isSubSampledSchool;
    uint32_t        unused2[5];
} nara_school_t;

#endif /* __NARA_SCHOOL_H__ */
