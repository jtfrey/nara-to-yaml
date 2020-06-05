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
    nara_grade_none = 0,
    nara_grade_pre_k,
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
    nara_suspension_1_to_3_days = 0,
    nara_suspension_4_to_10_days,
    nara_suspension_11_or_more_days,
    nara_suspension_max
};

extern const char* nara_suspension_labels[nara_suspension_max];

enum {
    nara_special_ed_educable_mentally_retarded = 0,
    nara_special_ed_trainable_metally_retarded,
    nara_special_ed_serious_emotional_disturb,
    nara_special_ed_specific_learning_disabled,
    nara_special_ed_speech_impaired,
    nara_special_ed_orthopedically_handicapped,
    nara_special_ed_blind_or_visually_impaired,
    nara_special_ed_deaf_or_hard_of_hearing,
    nara_special_ed_other_health_impaired,
    nara_special_ed_multiple_handicaps,
    nara_special_ed_total_of_all_impaired_subtypes,
    nara_special_ed_gifted_or_talented,
    nara_special_ed_max
};

typedef struct {
    uint32_t        total[nara_ethnicity_max];
    uint32_t        byGender[nara_gender_max];
    uint32_t        lessThan10HrsPerWeek;
    uint32_t        moreThan10HrsPerWeekNotFullTime;
    uint32_t        fullTime;
} nara_special_ed_subtype_t;

extern const char* nara_special_ed_labels[nara_special_ed_max];

enum {
    nara_empl_status_full_time = 0,
    nara_empl_status_part_time,
    nara_empl_status_max
};

extern const char* nara_empl_status_labels[nara_empl_status_max];

enum {
    nara_assignment_class_first = 0,
    nara_assignment_class_middle,
    nara_assignment_class_last,
    nara_assignment_class_max
};

extern const char* nara_assignment_class_labels[nara_assignment_class_max];

typedef struct {
    uint32_t        gradeOrAge;
    uint32_t        subjectCode;
    uint32_t        pupils[nara_gender_max][nara_ethnicity_max];
} nara_pupil_assignment_t;

typedef struct {
    uint32_t                        systemOECode;
    uint32_t                        recordType;
    uint32_t                        selectionCode;
    char                            systemName[32];
    char                            systemCounty[16];
    char                            systemCity[16];
    char                            systemZipCode[8];
    uint32_t                        numSchoolsInSchoolSystem;
    uint32_t                        isInConsolidation;
    uint32_t                        isInUnification;
    uint32_t                        isInDivision;
    uint32_t                        isInAnnexation;
    uint32_t                        isNotInAnyStateOfChange;
    uint32_t                        isUnderCourtOrderToDesegregate;
    uint32_t                        doGenderGradRequirementsDiffer;
    uint32_t                        pupils[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsEnrolledVocationEd[nara_gender_max][nara_ethnicity_max];
    uint32_t                        numSchoolsWith5OrMoreVocationEdPrograms;
    uint32_t                        pupilsSuspendedAtLeastOneDay[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsSuspendedAtLeastOneDayTotal[nara_ethnicity_max];
    uint32_t                        pupilsPrimaryLangNotEnglishTotal[nara_ethnicity_max];
    uint32_t                        pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal[nara_ethnicity_max];
    uint32_t                        pupilsSpecialEdTotal[nara_ethnicity_max];
    uint32_t                        pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal[nara_ethnicity_max];
    uint32_t                        pupilsSpecialEdForGiftedOrTalentedTotal[nara_ethnicity_max];
    uint32_t                        pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal[nara_ethnicity_max];
    uint32_t                        residentSchoolAgeChildrenIdentifiedRequiringSpecialEd;
    uint32_t                        residentPupilsInSpecialEdOperatedWithOtherSchoolSystems;
    uint32_t                        residentPupilsInSpecialEdOperatedExclOtherSchoolSystem;
    uint32_t                        residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem;
    uint32_t                        nonResidentPupilsInSpecialEd;
    uint32_t                        residentSchoolAgeChildrenOutOfSchoolHandicappingCondition;
    uint32_t                        residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction;
    uint32_t                        residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds;
    uint32_t                        fullTimeTeachersAssignedToSpecialEd;
    uint32_t                        partTimeTeachersAssignedToSpecialEd;
    uint32_t                        hasOtherReportingDates;
    uint32_t                        unused1;
    uint32_t                        unused2;
    uint32_t                        isESAADistrict;
    float                           samplingWeight;
    uint32_t                        unused3[9];
    uint32_t                        schoolOECode;
    uint32_t                        unused4;
    char                            schoolName[32];
    uint32_t                        isGradeOffered[nara_grade_max];
    uint32_t                        firstGradeLevelOfferedOrYoungestAgeOfPupilsForUngradedSection;
    uint32_t                        lastGradeLevelOfferedOrOldestAgeOfPupilsForUngradedSection;
    uint32_t                        isSchoolCampusExclusivelySpecialEd;
    uint32_t                        numVocationEdProgramsAtSchool;
    uint32_t                        hasFacilOrEquipForHandicapGroundLevelRampsWithHandrail;
    uint32_t                        hasFacilOrEquipForHandicapSingleStoryOrElevator;
    uint32_t                        hasFacilOrEquipForHandicapToiletStalls;
    uint32_t                        hasFacilOrEquipForHandicapDoors32InOrMore;
    uint32_t                        hasFacilOrEquipForHandicapSimultWarningSignals;
    uint32_t                        isBldgOrFacilConstructedOrAlteredUsingFedAssist;
    uint32_t                        pupilsHandicapNeedingSpecialAccom;
    uint32_t                        pupilsPhysOrMentallyHandicappedReqTransport;
    uint32_t                        pupilsHandicappedRecvPublicSubsidizedTransport;
    uint32_t                        doesTransportAccomodateWheelchairs;
    uint32_t                        pupilsTransportedAtPublicExpense;
    uint32_t                        pupils6To9InHomeEc[nara_gender_max];
    uint32_t                        pupils6To9InIndustrialArts[nara_gender_max];
    uint32_t                        pupils6To9InSingleSexHomeEconOrIndustrialArts[nara_gender_max];
    uint32_t                        pupils7To12EnrolledInHighestLevelMath[nara_gender_max];
    uint32_t                        pupils7To12EnrolledInHighestLevelNatSci[nara_gender_max];
    uint32_t                        pupilsInMembership[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsDroppedOutOrDiscontinuedSchooling[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsDroppedOutOrDiscontinuedSchoolingTotal[nara_ethnicity_max];
    uint32_t                        pupilsRecvHighSchoolDiplomaOrEquivMale[nara_ethnicity_max];
    uint32_t                        doesNotAwardHighSchoolDiplomaOrEquiv;
    uint32_t                        pupilsRecvHighSchoolDiplomaOrEquivFemale[nara_ethnicity_max];
    uint32_t                        pupilsRecvHighSchoolDiplomaOrEquivTotal[nara_ethnicity_max];
    uint32_t                        pupilsInSchoolPrimaryLangNotEnglishTotal[nara_ethnicity_max];
    uint32_t                        pupilsInSchoolPrimaryLangNotEnglishInProgramsNotInEnglishTotal[nara_ethnicity_max];
    uint32_t                        hasPupilsWhoWereSuspendedOrExpelled;
    uint32_t                        pupilsSuspendedOneTimeOnly[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsSuspendedOneTimeOnlyTotal[nara_ethnicity_max];
    uint32_t                        pupilsSuspendedOneTimeOnlyByDayCountTotal[nara_suspension_max][nara_ethnicity_max];
    uint32_t                        pupilsSuspendedMoreThanOnce[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsSuspendedMoreThanOnceTotal[nara_ethnicity_max];
    uint32_t                        pupilsSuspendedMoreThanOnceDayCountTotal[nara_suspension_max][nara_ethnicity_max];
    uint32_t                        pupilsExpelled[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsExpelledTotal[nara_ethnicity_max];
    uint32_t                        pupilsRecvCorporalPunishAsFormalDiscipline[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsRecvCorporalPunishAsFormalDisciplineTotal[nara_ethnicity_max];
    uint32_t                        pupilsReferredForDisciplinaryActionToCourtOrJuvAuthor[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsReferredForDisciplinaryActionToCourtOrJuvAuthorTotal[nara_ethnicity_max];
    uint32_t                        pupilsReferredToAltEducProgAsFormalDiscipline[nara_gender_max][nara_ethnicity_max];
    uint32_t                        pupilsReferredToAltEducProgAsFormalDisciplineTotal[nara_ethnicity_max];
    uint32_t                        hasSpecialEdPrograms;
    nara_special_ed_subtype_t       pupilsInSpecialEd[nara_special_ed_max];
    uint32_t                        teachersAssignedToSpecialEdPrograms[nara_special_ed_total_of_all_impaired_subtypes + 1][nara_empl_status_max];
    uint32_t                        pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalented[nara_ethnicity_max - 1];
    uint32_t                        checkOnNumberOfFullTimeTeachers;
    uint32_t                        numFullTimeTeachers;
    uint32_t                        selectionNumber;
    nara_pupil_assignment_t         pupilAssignments[2][nara_assignment_class_max][3];
    uint32_t                        unused5[3];
    uint32_t                        pupilAssignmentsEndOfSpanOrSingleEntryGradeOrAge[2][nara_assignment_class_max][3];
    uint32_t                        unused6[5];
    uint32_t                        errorBitArray[43];
} nara_school_t;

#endif /* __NARA_SCHOOL_H__ */
