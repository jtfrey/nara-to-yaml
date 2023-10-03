/*
 * nara_school_impl
 *
 * The NARA "school" record implementation.
 *
 */

#include "nara_school.h"

const char* nara_grade_labels[nara_grade_max] = {
            "Ungraded",
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

const char* nara_suspension_labels[nara_suspension_max] = {
            "1 to 3 days",
            "4 to 10 days",
            "11 or more days"
        };

const char* nara_special_ed_labels[nara_special_ed_max] = {
            "Educable Mentally Retarded",
            "Trainable Mentally Retarded",
            "Serious Emotional Disturbance",
            "Specific Learning Disability",
            "Speech Impairment",
            "Orthopedic Handicap",
            "Blind or Visually Impaired",
            "Deaf or Hard of Hearing",
            "Other Health Impairment",
            "Multiple Handicaps",
            "Total of All Impairments",
            "Gifted or Talented"
        };

const char* nara_empl_status_labels[nara_empl_status_max] = {
            "Full Time",
            "Part Time"
        };

const char* nara_assignment_class_labels[nara_assignment_class_max] = {
            "First",
            "Middle",
            "Last"
        };


typedef union {
    nara_school_t    break_out;
    struct {
        uint32_t        f1[3];
        char            systemName[32];
        char            systemCounty[16];
        char            systemCity[16];
        char            systemZipCode[8];
        uint32_t        f2[8 + 2 * nara_ethnicity_max * nara_gender_max + 1 + nara_ethnicity_max * nara_gender_max + 7 * nara_ethnicity_max + 14];
        float           f3;
        uint32_t        f4[11];
        char            schoolName[32];
        uint32_t        f5[nara_grade_max + 15 + 5 * nara_gender_max + 2 * nara_gender_max * nara_ethnicity_max +
                           2 * nara_ethnicity_max + 1 + 4 * nara_ethnicity_max + 1 + nara_gender_max * nara_ethnicity_max +
                           nara_ethnicity_max + nara_suspension_max * nara_ethnicity_max + nara_gender_max * nara_ethnicity_max +
                           nara_ethnicity_max + nara_suspension_max * nara_ethnicity_max + 4 * (nara_gender_max + 1) * nara_ethnicity_max +
                           1 + nara_special_ed_max * (sizeof(nara_special_ed_subtype_t) / sizeof(uint32_t)) + 
                           nara_special_ed_total_of_all_impaired_subtypes * nara_empl_status_max + nara_empl_status_max +
                           (nara_ethnicity_max - 1) + 3 + 2 * nara_assignment_class_max * 3 * (sizeof(nara_pupil_assignment_t) / sizeof(uint32_t)) +
                           3 + 2 * nara_assignment_class_max * 3 + 48];
    } compact;
} nara_school_internal_t;

/**/

int
__nara_record_is_type_school(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == NARA_1976_RECORD_SIZE) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_school) );
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
    
    iMax = sizeof(school->compact.f4) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) school->compact.f4[i] = nara_be_to_host_32(school->compact.f4[i]);
    
    iMax = sizeof(school->compact.f5) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) school->compact.f5[i] = nara_be_to_host_32(school->compact.f5[i]);
    
    TRANSCODE(school, systemName, "district name");
    TRANSCODE(school, systemCounty, "district county");
    TRANSCODE(school, systemCity, "district city");
    TRANSCODE(school, systemZipCode, "district zip code");
    TRANSCODE(school, schoolName, "school name");
    
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
                        "systemName,"
                        "systemCounty,"
                        "systemCity,"
                        "systemZipCode,"
                        "numSchoolsInSchoolSystem,"
                        "isInConsolidation,"
                        "isInUnification,"
                        "isInDivision,"
                        "isInAnnexation,"
                        "isNotInAnyStateOfChange,"
                        "isUnderCourtOrderToDesegregate,"
                        "doGenderGradRequirementsDiffer,"
                        "numSchoolsWith5OrMoreVocationEdPrograms,"
                        "residentSchoolAgeChildrenIdentifiedRequiringSpecialEd,"
                        "residentPupilsInSpecialEdOperatedWithOtherSchoolSystems,"
                        "residentPupilsInSpecialEdOperatedExclOtherSchoolSystem,"
                        "residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem,"
                        "nonResidentPupilsInSpecialEd,"
                        "residentSchoolAgeChildrenOutOfSchoolHandicappingCondition,"
                        "residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction,"
                        "residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds,"
                        "fullTimeTeachersAssignedToSpecialEd,"
                        "partTimeTeachersAssignedToSpecialEd,"
                        "hasOtherReportingDates,"
                        "isESAADistrict,"
                        "samplingWeight,"
                    );
                    
                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupils_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);
                
                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsEnrolledVocationEd_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);
                
                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedAtLeastOneDay_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedAtLeastOneDayTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsPrimaryLangNotEnglishTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsSpecialEdTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsSpecialEdForGiftedOrTalentedTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal_%s\",", nara_ethnicity_labels[i]);
                    
                fprintf(CONTEXT->schoolFptr,
                        "schoolOECode,"
                        "schoolName,"
                        "firstGradeLevelOfferedOrYoungestAgeOfPupilsForUngradedSection,"
                        "lastGradeLevelOfferedOrOldestAgeOfPupilsForUngradedSection,"
                        "isSchoolCampusExclusivelySpecialEd,"
                        "numVocationEdProgramsAtSchool,"
                        "hasFacilOrEquipForHandicapGroundLevelRampsWithHandrail,"
                        "hasFacilOrEquipForHandicapSingleStoryOrElevator,"
                        "hasFacilOrEquipForHandicapToiletStalls,"
                        "hasFacilOrEquipForHandicapDoors32InOrMore,"
                        "hasFacilOrEquipForHandicapSimultWarningSignals,"
                        "isBldgOrFacilConstructedOrAlteredUsingFedAssist,"
                        "pupilsHandicapNeedingSpecialAccom,"
                        "pupilsPhysOrMentallyHandicappedReqTransport,"
                        "pupilsHandicappedRecvPublicSubsidizedTransport,"
                        "doesTransportAccomodateWheelchairs,"
                        "pupilsTransportedAtPublicExpense,"
                        "doesNotAwardHighSchoolDiplomaOrEquiv,"
                        "hasPupilsWhoWereSuspendedOrExpelled,"
                        "hasSpecialEdPrograms,"
                        "checkOnNumberOfFullTimeTeachers,"
                        "numFullTimeTeachers,"
                        "selectionNumber,"
                    );

                for ( i = 0; i < nara_grade_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"isGradeOffered_%s\",", nara_grade_labels[i]);

                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupils6To9InHomeEc_%s\",", nara_gender_labels[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupils6To9InIndustrialArts_%s\",", nara_gender_labels[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupils6To9InSingleSexHomeEconOrIndustrialArts_%s\",", nara_gender_labels[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupils7To12EnrolledInHighestLevelMath_%s\",", nara_gender_labels[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupils7To12EnrolledInHighestLevelNatSci_%s\",", nara_gender_labels[i]);
                    
                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsInMembership_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsDroppedOutOrDiscontinuedSchooling_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsDroppedOutOrDiscontinuedSchoolingTotal_%s\",", nara_grade_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsRecvHighSchoolDiplomaOrEquivMale_%s\",", nara_grade_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsRecvHighSchoolDiplomaOrEquivFemale_%s\",", nara_grade_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsRecvHighSchoolDiplomaOrEquivTotal_%s\",", nara_grade_labels[i]);
                    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsInSchoolPrimaryLangNotEnglishTotal_%s\",", nara_grade_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsInSchoolPrimaryLangNotEnglishInProgramsNotInEnglishTotal_%s\",", nara_grade_labels[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedOneTimeOnly_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedOneTimeOnlyTotal_%s\",", nara_grade_labels[i]);

                for ( j = 0; j < nara_suspension_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedOneTimeOnlyByDayCountTotal_%s_%s\",", nara_suspension_labels[j], nara_ethnicity_labels[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedMoreThanOnce_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedMoreThanOnceTotal_%s\",", nara_grade_labels[i]);

                for ( j = 0; j < nara_suspension_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsSuspendedMoreThanOnceDayCountTotal_%s_%s\",", nara_suspension_labels[j], nara_ethnicity_labels[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsExpelled_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsExpelledTotal_%s\",", nara_grade_labels[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsRecvCorporalPunishAsFormalDiscipline_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsRecvCorporalPunishAsFormalDisciplineTotal_%s\",", nara_grade_labels[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsReferredForDisciplinaryActionToCourtOrJuvAuthor_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsReferredForDisciplinaryActionToCourtOrJuvAuthorTotal_%s\",", nara_grade_labels[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsReferredToAltEducProgAsFormalDiscipline_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsReferredToAltEducProgAsFormalDisciplineTotal_%s\",", nara_grade_labels[i]);

                for ( j = 0; j < nara_special_ed_max; j++ ) {
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsInSpecialEd_total_%s_%s\",", nara_special_ed_labels[j], nara_ethnicity_labels[i]);
                    for ( i = 0; i < nara_gender_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"pupilsInSpecialEd_byGender_%s_%s\",", nara_special_ed_labels[j], nara_gender_labels[i]);
                    fprintf(CONTEXT->schoolFptr, "\"pupilsInSpecialEd_lessThan10HrsPerWeek_%s\",", nara_special_ed_labels[j]);
                    fprintf(CONTEXT->schoolFptr, "\"pupilsInSpecialEd_moreThan10HrsPerWeekNotFullTime_%s\",", nara_special_ed_labels[j]);
                    fprintf(CONTEXT->schoolFptr, "\"pupilsInSpecialEd_fullTime_%s\",", nara_special_ed_labels[j]);
                }
                
                for ( j = 0; j < nara_special_ed_total_of_all_impaired_subtypes; j++ )
                    for ( i = 0; i < nara_empl_status_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "\"teachersAssignedToSpecialEdPrograms_%s_%s\",", nara_special_ed_labels[j], nara_empl_status_labels[i]);
                for ( i = 0; i < nara_empl_status_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"teachersAssignedToSpecialEdPrograms_%s_%s\",", nara_special_ed_labels[nara_special_ed_gifted_or_talented], nara_empl_status_labels[i]);
                
                for ( i = 0; i < nara_ethnicity_max - 1; i++ )
                    fprintf(CONTEXT->schoolFptr, "\"pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalented_%s\",", nara_grade_labels[i]);

                idx = 1;
                for ( m = 0; m < 2; m++ ) {
                    for ( l = 0; l < nara_assignment_class_max; l++ ) {
                        for ( k = 0; k < 3; k++ ) {
                            fprintf(CONTEXT->schoolFptr, "\"pupilAssignments_%d_gradeOrAge_%s\",", idx, nara_assignment_class_labels[l]);
                            fprintf(CONTEXT->schoolFptr, "\"pupilAssignments_%d_subjectCode_%s\",", idx, nara_assignment_class_labels[l]);
                            for ( j = 0; j < nara_gender_max; j++ )
                                for ( i = 0; i < nara_ethnicity_max; i++ )
                                    fprintf(CONTEXT->schoolFptr, "\"pupilAssignments_%d_gradeOrAge_pupils_%s_%s_%s\",", idx, nara_assignment_class_labels[l], nara_gender_labels[j], nara_ethnicity_labels[i]);
                            idx++;
                        }
                    }
                }
                
                idx = 1;
                for ( k = 0; k < 2; k++ )
                    for ( j = 0; j < nara_assignment_class_max; j++ )
                        for ( i = 0; i < 3; i++ )
                            fprintf(CONTEXT->schoolFptr, "\"pupilAssignmentsEndOfSpanOrSingleEntryGradeOrAge_%d_%s\",", idx++, nara_assignment_class_labels[j]);
                
                for ( i = 1; i <= 42; i++ )
                    fprintf(CONTEXT->schoolFptr, "errorBitArray_%d,", i);
                fprintf(CONTEXT->schoolFptr, "errorBitArray_%d\n", i);
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
    
    LOCAL_STR_DECL(systemName, sizeof(school->systemName));
    LOCAL_STR_DECL(systemCounty, sizeof(school->systemCounty));
    LOCAL_STR_DECL(systemCity, sizeof(school->systemCity));
    LOCAL_STR_DECL(systemZipCode, sizeof(school->systemZipCode));
    LOCAL_STR_DECL(schoolName, sizeof(school->schoolName));
    
    LOCAL_STR_FILL(systemName, sizeof(school->systemName), school->systemName);
    LOCAL_STR_FILL(systemCounty, sizeof(school->systemCounty), school->systemCounty);
    LOCAL_STR_FILL(systemCity, sizeof(school->systemCity), school->systemCity);
    LOCAL_STR_FILL(systemZipCode, sizeof(school->systemZipCode), school->systemZipCode);
    LOCAL_STR_FILL(schoolName, sizeof(school->schoolName), school->schoolName);
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
            
            if ( CONTEXT->fptr ) {
                fprintf(CONTEXT->fptr,
                        "- recordType: school\n"
                        "  systemOECode: %u\n"
                        "  selectionCode: %u\n"
                        "  systemName: %s\n"
                        "  systemCounty: %s\n"
                        "  systemCity: %s\n"
                        "  systemZipCode: %s\n"
                        "  numSchoolsInSchoolSystem: %u\n"
                        "  isInConsolidation: %u\n"
                        "  isInUnification: %u\n"
                        "  isInDivision: %u\n"
                        "  isInAnnexation: %u\n"
                        "  isNotInAnyStateOfChange: %u\n"
                        "  isUnderCourtOrderToDesegregate: %u\n"
                        "  doGenderGradRequirementsDiffer: %u\n"
                        "  numSchoolsWith5OrMoreVocationEdPrograms: %u\n"
                        "  residentSchoolAgeChildrenIdentifiedRequiringSpecialEd: %u\n"
                        "  residentPupilsInSpecialEdOperatedWithOtherSchoolSystems: %u\n"
                        "  residentPupilsInSpecialEdOperatedExclOtherSchoolSystem: %u\n"
                        "  residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem: %u\n"
                        "  nonResidentPupilsInSpecialEd: %u\n"
                        "  residentSchoolAgeChildrenOutOfSchoolHandicappingCondition: %u\n"
                        "  residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction: %u\n"
                        "  residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds: %u\n"
                        "  fullTimeTeachersAssignedToSpecialEd: %u\n"
                        "  partTimeTeachersAssignedToSpecialEd: %u\n"
                        "  hasOtherReportingDates: %u\n"
                        "  isESAADistrict: %u\n"
                        "  samplingWeight: %g\n",
                        school->systemOECode,
                        school->selectionCode,
                        systemName,
                        systemCounty,
                        systemCity,
                        systemZipCode,
                        school->numSchoolsInSchoolSystem,
                        school->isInConsolidation,
                        school->isInUnification,
                        school->isInDivision,
                        school->isInAnnexation,
                        school->isNotInAnyStateOfChange,
                        school->isUnderCourtOrderToDesegregate,
                        school->doGenderGradRequirementsDiffer,
                        school->numSchoolsWith5OrMoreVocationEdPrograms,
                        school->residentSchoolAgeChildrenIdentifiedRequiringSpecialEd,
                        school->residentPupilsInSpecialEdOperatedWithOtherSchoolSystems,
                        school->residentPupilsInSpecialEdOperatedExclOtherSchoolSystem,
                        school->residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem,
                        school->nonResidentPupilsInSpecialEd,
                        school->residentSchoolAgeChildrenOutOfSchoolHandicappingCondition,
                        school->residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction,
                        school->residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds,
                        school->fullTimeTeachersAssignedToSpecialEd,
                        school->partTimeTeachersAssignedToSpecialEd,
                        school->hasOtherReportingDates,
                        school->isESAADistrict,
                        school->samplingWeight
                    );

                fprintf(CONTEXT->fptr, "  pupils:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n",  nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupils[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsEnrolledVocationEd:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n",  nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsEnrolledVocationEd[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsSuspendedAtLeastOneDay:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n",  nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSuspendedAtLeastOneDay[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsSuspendedAtLeastOneDayTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSuspendedAtLeastOneDayTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsPrimaryLangNotEnglishTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsPrimaryLangNotEnglishTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSpecialEdTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSpecialEdTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSpecialEdForGiftedOrTalentedTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSpecialEdForGiftedOrTalentedTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal[i]);
                    
                    
                fprintf(CONTEXT->fptr,
                        "  schoolOECode: %u\n"
                        "  schoolName: \"%s\"\n"
                        "  firstGradeLevelOfferedOrYoungestAgeOfPupilsForUngradedSection: %u\n"
                        "  lastGradeLevelOfferedOrOldestAgeOfPupilsForUngradedSection: %u\n"
                        "  isSchoolCampusExclusivelySpecialEd: %u\n"
                        "  numVocationEdProgramsAtSchool: %u\n"
                        "  hasFacilOrEquipForHandicapGroundLevelRampsWithHandrail: %u\n"
                        "  hasFacilOrEquipForHandicapSingleStoryOrElevator: %u\n"
                        "  hasFacilOrEquipForHandicapToiletStalls: %u\n"
                        "  hasFacilOrEquipForHandicapDoors32InOrMore: %u\n"
                        "  hasFacilOrEquipForHandicapSimultWarningSignals: %u\n"
                        "  isBldgOrFacilConstructedOrAlteredUsingFedAssist: %u\n"
                        "  pupilsHandicapNeedingSpecialAccom: %u\n"
                        "  pupilsPhysOrMentallyHandicappedReqTransport: %u\n"
                        "  pupilsHandicappedRecvPublicSubsidizedTransport: %u\n"
                        "  doesTransportAccomodateWheelchairs: %u\n"
                        "  pupilsTransportedAtPublicExpense: %u\n"
                        "  doesNotAwardHighSchoolDiplomaOrEquiv: %u\n"
                        "  hasPupilsWhoWereSuspendedOrExpelled: %u\n"
                        "  hasSpecialEdPrograms: %u\n"
                        "  checkOnNumberOfFullTimeTeachers: %u\n"
                        "  numFullTimeTeachers: %u\n"
                        "  selectionNumber: %u\n",
                        school->schoolOECode,
                        schoolName,
                        school->firstGradeLevelOfferedOrYoungestAgeOfPupilsForUngradedSection,
                        school->lastGradeLevelOfferedOrOldestAgeOfPupilsForUngradedSection,
                        school->isSchoolCampusExclusivelySpecialEd,
                        school->numVocationEdProgramsAtSchool,
                        school->hasFacilOrEquipForHandicapGroundLevelRampsWithHandrail,
                        school->hasFacilOrEquipForHandicapSingleStoryOrElevator,
                        school->hasFacilOrEquipForHandicapToiletStalls,
                        school->hasFacilOrEquipForHandicapDoors32InOrMore,
                        school->hasFacilOrEquipForHandicapSimultWarningSignals,
                        school->isBldgOrFacilConstructedOrAlteredUsingFedAssist,
                        school->pupilsHandicapNeedingSpecialAccom,
                        school->pupilsPhysOrMentallyHandicappedReqTransport,
                        school->pupilsHandicappedRecvPublicSubsidizedTransport,
                        school->doesTransportAccomodateWheelchairs,
                        school->pupilsTransportedAtPublicExpense,
                        school->doesNotAwardHighSchoolDiplomaOrEquiv,
                        school->hasPupilsWhoWereSuspendedOrExpelled,
                        school->hasSpecialEdPrograms,
                        school->checkOnNumberOfFullTimeTeachers,
                        school->numFullTimeTeachers,
                        school->selectionNumber
                    );

                fprintf(CONTEXT->fptr, "  isGradeOffered:\n");
                for ( i = 0; i < nara_grade_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->isGradeOffered[i]);

                fprintf(CONTEXT->fptr, "  pupils6To9InHomeEc:\n");
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_gender_labels[i], school->pupils6To9InHomeEc[i]);
                    
                fprintf(CONTEXT->fptr, "  pupils6To9InIndustrialArts:\n");
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_gender_labels[i], school->pupils6To9InIndustrialArts[i]);
                    
                fprintf(CONTEXT->fptr, "  pupils6To9InSingleSexHomeEconOrIndustrialArts:\n");
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_gender_labels[i], school->pupils6To9InSingleSexHomeEconOrIndustrialArts[i]);
                    
                fprintf(CONTEXT->fptr, "  pupils7To12EnrolledInHighestLevelMath:\n");
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_gender_labels[i], school->pupils7To12EnrolledInHighestLevelMath[i]);
                    
                fprintf(CONTEXT->fptr, "  pupils7To12EnrolledInHighestLevelNatSci:\n");
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_gender_labels[i], school->pupils7To12EnrolledInHighestLevelNatSci[i]);
                    
                fprintf(CONTEXT->fptr, "  pupilsInMembership:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsInMembership[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsDroppedOutOrDiscontinuedSchooling:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsDroppedOutOrDiscontinuedSchooling[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsDroppedOutOrDiscontinuedSchoolingTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsDroppedOutOrDiscontinuedSchoolingTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsRecvHighSchoolDiplomaOrEquivMale:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsRecvHighSchoolDiplomaOrEquivMale[i]);

                fprintf(CONTEXT->fptr, "  pupilsRecvHighSchoolDiplomaOrEquivFemale:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsRecvHighSchoolDiplomaOrEquivFemale[i]);

                fprintf(CONTEXT->fptr, "  pupilsRecvHighSchoolDiplomaOrEquivTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsRecvHighSchoolDiplomaOrEquivTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsInSchoolPrimaryLangNotEnglishTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsInSchoolPrimaryLangNotEnglishTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsInSchoolPrimaryLangNotEnglishInProgramsNotInEnglishTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsInSchoolPrimaryLangNotEnglishInProgramsNotInEnglishTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSuspendedOneTimeOnly:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSuspendedOneTimeOnly[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsSuspendedOneTimeOnlyTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsSuspendedOneTimeOnlyTotal[i]);


                fprintf(CONTEXT->fptr, "  pupilsSuspendedOneTimeOnlyByDayCountTotal:\n");
                for ( j = 0; j < nara_suspension_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_suspension_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSuspendedOneTimeOnlyByDayCountTotal[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsSuspendedMoreThanOnce:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSuspendedMoreThanOnce[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsSuspendedMoreThanOnceTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsSuspendedMoreThanOnceTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSuspendedMoreThanOnceDayCountTotal:\n");
                for ( j = 0; j < nara_suspension_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_suspension_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsSuspendedMoreThanOnceDayCountTotal[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsExpelled:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsExpelled[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsExpelledTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsExpelledTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsRecvCorporalPunishAsFormalDiscipline:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsRecvCorporalPunishAsFormalDiscipline[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsRecvCorporalPunishAsFormalDisciplineTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsRecvCorporalPunishAsFormalDisciplineTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsReferredForDisciplinaryActionToCourtOrJuvAuthor:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsReferredForDisciplinaryActionToCourtOrJuvAuthor[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsReferredForDisciplinaryActionToCourtOrJuvAuthorTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsReferredForDisciplinaryActionToCourtOrJuvAuthorTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsReferredToAltEducProgAsFormalDiscipline:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsReferredToAltEducProgAsFormalDiscipline[j][i]);
                }

                fprintf(CONTEXT->fptr, "  pupilsReferredToAltEducProgAsFormalDisciplineTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsReferredToAltEducProgAsFormalDisciplineTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsInSpecialEd:\n");
                for ( j = 0; j < nara_special_ed_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_special_ed_labels[j]);
                    fprintf(CONTEXT->fptr, "      total:\n");
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "         \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilsInSpecialEd[j].total[i]);
                    fprintf(CONTEXT->fptr, "      byGender:\n");
                    for ( i = 0; i < nara_gender_max; i++ )
                        fprintf(CONTEXT->fptr, "         \"%s\": %u\n", nara_gender_labels[i], school->pupilsInSpecialEd[j].byGender[i]);
                    fprintf(CONTEXT->fptr, "      lessThan10HrsPerWeek: %u\n", school->pupilsInSpecialEd[j].lessThan10HrsPerWeek);
                    fprintf(CONTEXT->fptr, "      moreThan10HrsPerWeekNotFullTime: %u\n", school->pupilsInSpecialEd[j].moreThan10HrsPerWeekNotFullTime);
                    fprintf(CONTEXT->fptr, "      fullTime: %u\n", school->pupilsInSpecialEd[j].fullTime);
                }

                fprintf(CONTEXT->fptr, "  teachersAssignedToSpecialEdPrograms:\n");
                for ( j = 0; j < nara_special_ed_total_of_all_impaired_subtypes; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_special_ed_labels[j]);
                    for ( i = 0; i < nara_empl_status_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_empl_status_labels[i], school->teachersAssignedToSpecialEdPrograms[j][i]);
                }
                fprintf(CONTEXT->fptr, "    \"%s\":\n", nara_special_ed_labels[nara_special_ed_gifted_or_talented]);
                for ( i = 0; i < nara_empl_status_max; i++ )
                    fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_empl_status_labels[i], school->teachersAssignedToSpecialEdPrograms[j][i]);
                
                fprintf(CONTEXT->fptr, "  pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalented:\n");
                for ( i = 0; i < nara_ethnicity_max - 1; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_grade_labels[i], school->pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalented[i]);

                fprintf(CONTEXT->fptr, "  pupilAssignments:\n");
                for ( m = 0; m < 2; m++ ) {
                    for ( l = 0; l < nara_assignment_class_max; l++ ) {
                        fprintf(CONTEXT->fptr, "    - \"%s\":\n", nara_assignment_class_labels[l]);
                        for ( k = 0; k < 3; k++ ) {
                            fprintf(CONTEXT->fptr, "      - gradeOrAge: %u\n", school->pupilAssignments[m][l][k].gradeOrAge);
                            fprintf(CONTEXT->fptr, "        subjectCode: %u\n", school->pupilAssignments[m][l][k].subjectCode);
                            fprintf(CONTEXT->fptr, "        pupils:\n");
                            for ( j = 0; j < nara_gender_max; j++ ) {
                                fprintf(CONTEXT->fptr, "          \"%s\":\n", nara_gender_labels[j]);
                                for ( i = 0; i < nara_ethnicity_max; i++ )
                                    fprintf(CONTEXT->fptr, "            \"%s\": %u\n", nara_ethnicity_labels[i], school->pupilAssignments[m][l][k].pupils[j][i]);
                            }
                        }
                    }
                }

                fprintf(CONTEXT->fptr, "  pupilAssignmentsEndOfSpanOrSingleEntryGradeOrAge:\n");
                for ( k = 0; k < 2; k++ ) {
                    for ( j = 0; j < nara_assignment_class_max; j++ ) {
                        fprintf(CONTEXT->fptr, "    - \"%s\":\n", nara_assignment_class_labels[j]);
                        for ( i = 0; i < 3; i++ )
                            fprintf(CONTEXT->fptr, "      - %u\n", school->pupilAssignmentsEndOfSpanOrSingleEntryGradeOrAge[k][j][i]);
                    }
                }

                fprintf(CONTEXT->fptr, "  errorBitArray:\n");
                for ( i = 0; i < 43; i++ )
                    fprintf(CONTEXT->fptr, "    - %u\n", school->errorBitArray[i]);
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
                        "\"%s\","
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%g,",
                        school->systemOECode,
                        school->selectionCode,
                        systemName,
                        systemCounty,
                        systemCity,
                        systemZipCode,
                        school->numSchoolsInSchoolSystem,
                        school->isInConsolidation,
                        school->isInUnification,
                        school->isInDivision,
                        school->isInAnnexation,
                        school->isNotInAnyStateOfChange,
                        school->isUnderCourtOrderToDesegregate,
                        school->doGenderGradRequirementsDiffer,
                        school->numSchoolsWith5OrMoreVocationEdPrograms,
                        school->residentSchoolAgeChildrenIdentifiedRequiringSpecialEd,
                        school->residentPupilsInSpecialEdOperatedWithOtherSchoolSystems,
                        school->residentPupilsInSpecialEdOperatedExclOtherSchoolSystem,
                        school->residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem,
                        school->nonResidentPupilsInSpecialEd,
                        school->residentSchoolAgeChildrenOutOfSchoolHandicappingCondition,
                        school->residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction,
                        school->residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds,
                        school->fullTimeTeachersAssignedToSpecialEd,
                        school->partTimeTeachersAssignedToSpecialEd,
                        school->hasOtherReportingDates,
                        school->isESAADistrict,
                        school->samplingWeight
                    );

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupils[j][i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsEnrolledVocationEd[j][i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedAtLeastOneDay[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedAtLeastOneDayTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsPrimaryLangNotEnglishTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSpecialEdTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSpecialEdForGiftedOrTalentedTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal[i]);
                    
                fprintf(CONTEXT->schoolFptr,
                        "%u,"
                        "\"%s\","
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,",
                        school->schoolOECode,
                        schoolName,
                        school->firstGradeLevelOfferedOrYoungestAgeOfPupilsForUngradedSection,
                        school->lastGradeLevelOfferedOrOldestAgeOfPupilsForUngradedSection,
                        school->isSchoolCampusExclusivelySpecialEd,
                        school->numVocationEdProgramsAtSchool,
                        school->hasFacilOrEquipForHandicapGroundLevelRampsWithHandrail,
                        school->hasFacilOrEquipForHandicapSingleStoryOrElevator,
                        school->hasFacilOrEquipForHandicapToiletStalls,
                        school->hasFacilOrEquipForHandicapDoors32InOrMore,
                        school->hasFacilOrEquipForHandicapSimultWarningSignals,
                        school->isBldgOrFacilConstructedOrAlteredUsingFedAssist,
                        school->pupilsHandicapNeedingSpecialAccom,
                        school->pupilsPhysOrMentallyHandicappedReqTransport,
                        school->pupilsHandicappedRecvPublicSubsidizedTransport,
                        school->doesTransportAccomodateWheelchairs,
                        school->pupilsTransportedAtPublicExpense,
                        school->doesNotAwardHighSchoolDiplomaOrEquiv,
                        school->hasPupilsWhoWereSuspendedOrExpelled,
                        school->hasSpecialEdPrograms,
                        school->checkOnNumberOfFullTimeTeachers,
                        school->numFullTimeTeachers,
                        school->selectionNumber
                    );

                for ( i = 0; i < nara_grade_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->isGradeOffered[i]);

                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupils6To9InHomeEc[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupils6To9InIndustrialArts[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupils6To9InSingleSexHomeEconOrIndustrialArts[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupils7To12EnrolledInHighestLevelMath[i]);
                    
                for ( i = 0; i < nara_gender_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupils7To12EnrolledInHighestLevelNatSci[i]);
                    
                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInMembership[j][i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsDroppedOutOrDiscontinuedSchooling[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsDroppedOutOrDiscontinuedSchoolingTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsRecvHighSchoolDiplomaOrEquivMale[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsRecvHighSchoolDiplomaOrEquivFemale[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsRecvHighSchoolDiplomaOrEquivTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInSchoolPrimaryLangNotEnglishTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInSchoolPrimaryLangNotEnglishInProgramsNotInEnglishTotal[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedOneTimeOnly[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedOneTimeOnlyTotal[i]);

                for ( j = 0; j < nara_suspension_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedOneTimeOnlyByDayCountTotal[j][i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedMoreThanOnce[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedMoreThanOnceTotal[i]);

                for ( j = 0; j < nara_suspension_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsSuspendedMoreThanOnceDayCountTotal[j][i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsExpelled[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsExpelledTotal[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsRecvCorporalPunishAsFormalDiscipline[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsRecvCorporalPunishAsFormalDisciplineTotal[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsReferredForDisciplinaryActionToCourtOrJuvAuthor[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsReferredForDisciplinaryActionToCourtOrJuvAuthorTotal[i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsReferredToAltEducProgAsFormalDiscipline[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsReferredToAltEducProgAsFormalDisciplineTotal[i]);

                for ( j = 0; j < nara_special_ed_max; j++ ) {
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInSpecialEd[j].total[i]);
                    for ( i = 0; i < nara_gender_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInSpecialEd[j].byGender[i]);
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInSpecialEd[j].lessThan10HrsPerWeek);
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInSpecialEd[j].moreThan10HrsPerWeekNotFullTime);
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsInSpecialEd[j].fullTime);
                }

                for ( j = 0; j < nara_special_ed_total_of_all_impaired_subtypes; j++ )
                    for ( i = 0; i < nara_empl_status_max; i++ )
                        fprintf(CONTEXT->schoolFptr, "%u,", school->teachersAssignedToSpecialEdPrograms[j][i]);
                for ( i = 0; i < nara_empl_status_max; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->teachersAssignedToSpecialEdPrograms[j][i]);
                
                for ( i = 0; i < nara_ethnicity_max - 1; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalented[i]);

                for ( m = 0; m < 2; m++ ) {
                    for ( l = 0; l < nara_assignment_class_max; l++ ) {
                        for ( k = 0; k < 3; k++ ) {
                            fprintf(CONTEXT->schoolFptr, "%u,", school->pupilAssignments[m][l][k].gradeOrAge);
                            fprintf(CONTEXT->schoolFptr, "%u,", school->pupilAssignments[m][l][k].subjectCode);
                            for ( j = 0; j < nara_gender_max; j++ )
                                for ( i = 0; i < nara_ethnicity_max; i++ )
                                    fprintf(CONTEXT->schoolFptr, "%u,", school->pupilAssignments[m][l][k].pupils[j][i]);
                        }
                    }
                }

                for ( k = 0; k < 2; k++ )
                    for ( j = 0; j < nara_assignment_class_max; j++ )
                        for ( i = 0; i < 3; i++ )
                            fprintf(CONTEXT->schoolFptr, "%u,", school->pupilAssignmentsEndOfSpanOrSingleEntryGradeOrAge[k][j][i]);
                
                for ( i = 0; i < 42; i++ )
                    fprintf(CONTEXT->schoolFptr, "%u,", school->errorBitArray[i]);
                fprintf(CONTEXT->schoolFptr, "%u\n", school->errorBitArray[i]);
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
