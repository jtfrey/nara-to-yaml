/*
 * nara_district_impl
 *
 * The NARA "district" record implementation.
 *
 */

#include "nara_district.h"

const char* nara_ethnicity_labels[nara_ethnicity_max] = {
                "American or Alaskan Indian",
                "Asian or Pacific",
                "Black (not Hispanic)",
                "White (not Hispanic)",
                "Hispanic",
                "Total"
            };

const char* nara_gender_labels[nara_gender_max] = {
                "Male",
                "Female"
            };

typedef union {
    nara_district_t    break_out;
    struct {
        uint32_t        f1[3];
        char            systemName[32];
        char            systemCounty[16];
        char            systemCity[16];
        char            systemZipCode[8];
        uint32_t        f2[8 + 2 * nara_ethnicity_max * nara_gender_max + 1 + nara_ethnicity_max * nara_gender_max + 7 * nara_ethnicity_max + 14];
        float           f3;
        /*
         * We won't bother byte-swapping all these; the two "unused#" fields are right amid
         * all the other fields, so we won't worry about omitting them.
         */
        uint32_t        unused3[706];
        uint32_t        f4[43];
    } compact;
} nara_district_internal_t;

/**/

int
__nara_record_is_type_district(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == NARA_1976_RECORD_SIZE) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_district) );
}

/**/

nara_record_t*
__nara_record_process_district(
    nara_record_t*      theRecord
)
{
    nara_district_internal_t    *district = (nara_district_internal_t*)theRecord;
    char                        convBuffer[64];
    char                        *fromPtr, *toPtr;
    size_t                      fromLen, toLen;
    
    unsigned int                i, iMax;
    
    iMax = sizeof(district->compact.f1) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) district->compact.f1[i] = nara_be_to_host_32(district->compact.f1[i]);
    
    iMax = sizeof(district->compact.f2) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) district->compact.f2[i] = nara_be_to_host_32(district->compact.f2[i]);
    
    district->compact.f3 = nara_be_to_host_float(district->compact.f3);
    
    iMax = sizeof(district->compact.f4) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) district->compact.f4[i] = nara_be_to_host_32(district->compact.f4[i]);
    
    TRANSCODE(district, systemName, "district name");
    TRANSCODE(district, systemCounty, "district county");
    TRANSCODE(district, systemCity, "district city");
    TRANSCODE(district, systemZipCode, "district zip code");
    
    return theRecord;
}

/**/

void
__nara_export_init_district(
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
            if ( CONTEXT->districtFptr ) {
                fprintf(CONTEXT->districtFptr,
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
                        fprintf(CONTEXT->districtFptr, "\"pupils_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);
                
                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->districtFptr, "\"pupilsEnrolledVocationEd_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);
                
                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->districtFptr, "\"pupilsSuspendedAtLeastOneDay_%s_%s\",", nara_gender_labels[j], nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsSuspendedAtLeastOneDayTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsPrimaryLangNotEnglishTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsSpecialEdTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsSpecialEdForGiftedOrTalentedTotal_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal_%s\",", nara_ethnicity_labels[i]);
                
                for ( i = 1; i <= 42; i++ )
                    fprintf(CONTEXT->districtFptr, "errorBitArray_%d,", i);
                fprintf(CONTEXT->districtFptr, "errorBitArray_%d\n", i);
            }
            break;
        }
    }
}

/**/

void
__nara_export_destroy_district(
    nara_export_context_t   exportContext
)
{
}

/**/

void
__nara_record_export_district(
    nara_export_context_t   exportContext,
    nara_record_t           *theRecord
)
{
    nara_district_t             *district = (nara_district_t*)theRecord;
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i, j;
    
    LOCAL_STR_DECL(systemName, sizeof(district->systemName));
    LOCAL_STR_DECL(systemCounty, sizeof(district->systemCounty));
    LOCAL_STR_DECL(systemCity, sizeof(district->systemCity));
    LOCAL_STR_DECL(systemZipCode, sizeof(district->systemZipCode));
    
    LOCAL_STR_FILL(systemName, sizeof(district->systemName), district->systemName);
    LOCAL_STR_FILL(systemCounty, sizeof(district->systemCounty), district->systemCounty);
    LOCAL_STR_FILL(systemCity, sizeof(district->systemCity), district->systemCity);
    LOCAL_STR_FILL(systemZipCode, sizeof(district->systemZipCode), district->systemZipCode);
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
            
            if ( CONTEXT->fptr ) {
                fprintf(CONTEXT->fptr,
                        "- recordType: district\n"
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
                        district->systemOECode,
                        district->selectionCode,
                        systemName,
                        systemCounty,
                        systemCity,
                        systemZipCode,
                        district->numSchoolsInSchoolSystem,
                        district->isInConsolidation,
                        district->isInUnification,
                        district->isInDivision,
                        district->isInAnnexation,
                        district->isNotInAnyStateOfChange,
                        district->isUnderCourtOrderToDesegregate,
                        district->doGenderGradRequirementsDiffer,
                        district->numSchoolsWith5OrMoreVocationEdPrograms,
                        district->residentSchoolAgeChildrenIdentifiedRequiringSpecialEd,
                        district->residentPupilsInSpecialEdOperatedWithOtherSchoolSystems,
                        district->residentPupilsInSpecialEdOperatedExclOtherSchoolSystem,
                        district->residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem,
                        district->nonResidentPupilsInSpecialEd,
                        district->residentSchoolAgeChildrenOutOfSchoolHandicappingCondition,
                        district->residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction,
                        district->residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds,
                        district->fullTimeTeachersAssignedToSpecialEd,
                        district->partTimeTeachersAssignedToSpecialEd,
                        district->hasOtherReportingDates,
                        district->isESAADistrict,
                        district->samplingWeight
                    );

                fprintf(CONTEXT->fptr, "  pupils:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n",  nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], district->pupils[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  pupilsEnrolledVocationEd:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n",  nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsEnrolledVocationEd[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  pupilsSuspendedAtLeastOneDay:\n");
                for ( j = 0; j < nara_gender_max; j++ ) {
                    fprintf(CONTEXT->fptr, "    \"%s\":\n",  nara_gender_labels[j]);
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->fptr, "      \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsSuspendedAtLeastOneDay[j][i]);
                }
                
                fprintf(CONTEXT->fptr, "  pupilsSuspendedAtLeastOneDayTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsSuspendedAtLeastOneDayTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsPrimaryLangNotEnglishTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsPrimaryLangNotEnglishTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSpecialEdTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsSpecialEdTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsSpecialEdForGiftedOrTalentedTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsSpecialEdForGiftedOrTalentedTotal[i]);

                fprintf(CONTEXT->fptr, "  pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal[i]);

                fprintf(CONTEXT->fptr, "  errorBitArray:\n");
                for ( i = 0; i < 43; i++ )
                    fprintf(CONTEXT->fptr, "    - %u\n", district->errorBitArray[i]);
            }            
            break;
        }
        
        case nara_export_format_csv: {
            nara_export_context_csv_t   *CONTEXT = (nara_export_context_csv_t*)exportContext;
            
            if ( CONTEXT->districtFptr ) {
                fprintf(CONTEXT->districtFptr,
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
                        district->systemOECode,
                        district->selectionCode,
                        systemName,
                        systemCounty,
                        systemCity,
                        systemZipCode,
                        district->numSchoolsInSchoolSystem,
                        district->isInConsolidation,
                        district->isInUnification,
                        district->isInDivision,
                        district->isInAnnexation,
                        district->isNotInAnyStateOfChange,
                        district->isUnderCourtOrderToDesegregate,
                        district->doGenderGradRequirementsDiffer,
                        district->numSchoolsWith5OrMoreVocationEdPrograms,
                        district->residentSchoolAgeChildrenIdentifiedRequiringSpecialEd,
                        district->residentPupilsInSpecialEdOperatedWithOtherSchoolSystems,
                        district->residentPupilsInSpecialEdOperatedExclOtherSchoolSystem,
                        district->residentPupilsInSpecialEdOperatedEntityNotPublicSchoolSystem,
                        district->nonResidentPupilsInSpecialEd,
                        district->residentSchoolAgeChildrenOutOfSchoolHandicappingCondition,
                        district->residentSchoolAgeChildrenOutOfSchoolHandicappingConditionHomeboundInstruction,
                        district->residentSchoolAgeChildrenEvaluatedForSpecialEdNeeds,
                        district->fullTimeTeachersAssignedToSpecialEd,
                        district->partTimeTeachersAssignedToSpecialEd,
                        district->hasOtherReportingDates,
                        district->isESAADistrict,
                        district->samplingWeight
                    );

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->districtFptr, "%u,", district->pupils[j][i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->districtFptr, "%u,", district->pupilsEnrolledVocationEd[j][i]);

                for ( j = 0; j < nara_gender_max; j++ )
                    for ( i = 0; i < nara_ethnicity_max; i++ )
                        fprintf(CONTEXT->districtFptr, "%u,", district->pupilsSuspendedAtLeastOneDay[j][i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsSuspendedAtLeastOneDayTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsPrimaryLangNotEnglishTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsPrimaryLangNotEnglishInProgramsNotInEnglishTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsSpecialEdTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsSpecialEdForEducableMetallyRetardedOrHandicappedTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsSpecialEdForGiftedOrTalentedTotal[i]);

                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsHonorsOrAdvPlaceOrEnrichmentIfNoGiftedOrTalentedTotal[i]);

                for ( i = 0; i < 42; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->errorBitArray[i]);
                fprintf(CONTEXT->districtFptr, "%u\n", district->errorBitArray[i]);
            }
            break;
        }
        
    }
}

/**/

nara_record_t*
__nara_record_destroy_district(
    nara_record_t       *theRecord
)
{
    free((void*)theRecord);
    return NULL;
}
    