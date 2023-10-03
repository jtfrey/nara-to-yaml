/*
 * nara_district_impl
 *
 * The NARA "district" record implementation.
 *
 */

#include "nara_district.h"

const char* nara_ethnicity_labels[nara_ethnicity_max] = {
                "American Indian",
                "Black",
                "Asian American",
                "Spanish Surname",
                "Other",
                "Total"
            };

typedef union {
    nara_district_t    break_out;
    struct {
        uint32_t        f1[3];
        char            systemName[32];
        char            systemStreetAddr[32];
        char            systemCity[16];
        char            systemCounty[16];
        char            systemState[16];
        char            systemZipCode[8];
        char            systemAdminOfficer[20];
        uint32_t        f2[1 + nara_ethnicity_max + 2 + nara_ethnicity_max + 3 + 3 * nara_ethnicity_max + 10];
        char            srgCode[16];
        uint32_t        f3[4 + 5 * nara_ethnicity_total + 1];
    } compact;
} nara_district_internal_t;

/**/

int
__nara_record_is_type_district(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == 472) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_district) );
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
    
    iMax = sizeof(district->compact.f3) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) district->compact.f3[i] = nara_be_to_host_32(district->compact.f3[i]);
    
    TRANSCODE(district, systemName, "district name");
    TRANSCODE(district, systemStreetAddr, "district street address");
    TRANSCODE(district, systemCity, "district city");
    TRANSCODE(district, systemCounty, "district county");
    TRANSCODE(district, systemState, "district state");
    TRANSCODE(district, systemZipCode, "district zip code");
    TRANSCODE(district, systemAdminOfficer, "district admin officer");
    TRANSCODE(district, srgCode, "district SRG code");
    
    return theRecord;
}

/**/

void
__nara_export_init_district(
    nara_export_context_t   exportContext
)
{
    nara_export_context_base_t  *BASE_CONTEXT = (nara_export_context_base_t*)exportContext;
    unsigned int                i;
    
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
                        "schoolSystemCode,"
                        "oeCode1970,"
                        "srgCode,"
                        "systemName,"
                        "systemStreetAddr,"
                        "systemCity,"
                        "systemCounty,"
                        "systemState,"
                        "systemZipCode,"
                        "systemAdminOfficer,"
                        "numSchoolCampusForms,"
                        "nonResidentPupils,"
                        "residentPupils,"
                        "residentPupilsInOtherSystem,"
                        "residentPupilsInNonpublicSchools,"
                        "residentSchoolAgeNotInSchool,"
                        "bilingualInstruction,"
                        "bilingualTeacherCount,"
                        "bilingualPupilCount,"
                        "bilingualInstructionMaterials,"
                        "newSchoolProperty,"
                        "newSchoolConstruction,"
                        "newSchoolCapacity,"
                        "newSchoolGreaterMinorityComposition,"
                        "year,"
                        "stateCode,"
                        "assurance,"
                        "litigationCode,"
                        "selectionCode,"
                        "samplingWeight,"
                    );
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"expelledPupilCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"systemTeacherCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"professionalStaffCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "\"professionalsInMoreThanOneSchool_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsInAnotherSystemCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsInNonPublicSchoolsCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsSchoolAgeNotInSchoolCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsNonResidentCounts_%s\",", nara_ethnicity_labels[i]);
    
                for ( i = 0; i < nara_ethnicity_total - 1; i++ )
                    fprintf(CONTEXT->districtFptr, "\"pupilsResidentCounts_%s\",", nara_ethnicity_labels[i]);
                fprintf(CONTEXT->districtFptr, "\"pupilsResidentCounts_%s\"\n", nara_ethnicity_labels[i]);
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
    unsigned int                i, iMax;
    
    LOCAL_STR_DECL(systemName, sizeof(district->systemName));
    LOCAL_STR_DECL(systemStreetAddr, sizeof(district->systemStreetAddr));
    LOCAL_STR_DECL(systemCity, sizeof(district->systemCity));
    LOCAL_STR_DECL(systemCounty, sizeof(district->systemCounty));
    LOCAL_STR_DECL(systemState, sizeof(district->systemState));
    LOCAL_STR_DECL(systemZipCode, sizeof(district->systemZipCode));
    LOCAL_STR_DECL(systemAdminOfficer, sizeof(district->systemAdminOfficer));
    LOCAL_STR_DECL(srgCode, sizeof(district->srgCode));
    
    LOCAL_STR_FILL(systemName, sizeof(district->systemName), district->systemName);
    LOCAL_STR_FILL(systemStreetAddr, sizeof(district->systemStreetAddr), district->systemStreetAddr);
    LOCAL_STR_FILL(systemCity, sizeof(district->systemCity), district->systemCity);
    LOCAL_STR_FILL(systemCounty, sizeof(district->systemCounty), district->systemCounty);
    LOCAL_STR_FILL(systemState, sizeof(district->systemState), district->systemState);
    LOCAL_STR_FILL(systemZipCode, sizeof(district->systemZipCode), district->systemZipCode);
    LOCAL_STR_FILL(systemAdminOfficer, sizeof(district->systemAdminOfficer), district->systemAdminOfficer);
    LOCAL_STR_FILL(srgCode, sizeof(district->srgCode), district->srgCode);
    
    switch ( BASE_CONTEXT->format ) {
        
        case nara_export_format_yaml: {
            nara_export_context_yaml_t  *CONTEXT = (nara_export_context_yaml_t*)exportContext;
            
            if ( CONTEXT->fptr ) {
                fprintf(CONTEXT->fptr,
                        "- recordType: district\n"
                        "  schoolSystemCode: %u\n"
                        "  oeCode1970: %u\n"
                        "  srgCode: \"%s\"\n"
                        "  systemName: \"%s\"\n"
                        "  systemStreetAddr: \"%s\"\n"
                        "  systemCity: \"%s\"\n"
                        "  systemCounty: \"%s\"\n"
                        "  systemState: \"%s\"\n"
                        "  systemZipCode: \"%s\"\n"
                        "  systemAdminOfficer: \"%s\"\n"
                        "  numSchoolCampusForms: %u\n"
                        "  nonResidentPupils: %u\n"
                        "  residentPupils: %u\n"
                        "  residentPupilsInOtherSystem: %u\n"
                        "  residentPupilsInNonpublicSchools: %u\n"
                        "  residentSchoolAgeNotInSchool: %u\n"
                        "  bilingualInstruction: %u\n"
                        "  bilingualTeacherCount: %u\n"
                        "  bilingualPupilCount: %u\n"
                        "  bilingualInstructionMaterials: %u\n"
                        "  newSchoolProperty: %u\n"
                        "  newSchoolConstruction: %u\n"
                        "  newSchoolCapacity: %u\n"
                        "  newSchoolGreaterMinorityComposition: %u\n"
                        "  year: %u\n"
                        "  stateCode: %u\n"
                        "  assurance: %u\n"
                        "  litigationCode: %u\n"
                        "  selectionCode: %u\n"
                        "  samplingWeight: %u\n",
                        district->schoolSystemCode,
                        district->oeCode1970,
                        srgCode,
                        systemName,
                        systemStreetAddr,
                        systemCity,
                        systemCounty,
                        systemState,
                        systemZipCode,
                        systemAdminOfficer,
                        district->numSchoolCampusForms,
                        district->nonResidentPupils,
                        district->residentPupils,
                        district->residentPupilsInOtherSystem,
                        district->residentPupilsInNonpublicSchools,
                        district->residentSchoolAgeNotInSchool,
                        district->bilingualInstruction,
                        district->bilingualTeacherCount,
                        district->bilingualPupilCount,
                        district->bilingualInstructionMaterials,
                        district->newSchoolProperty,
                        district->newSchoolConstruction,
                        district->newSchoolCapacity,
                        district->newSchoolGreaterMinorityComposition,
                        district->year,
                        district->stateCode,
                        district->assurance,
                        district->litigationCode,
                        district->selectionCode,
                        district->samplingWeight
                    );
        
                fprintf(CONTEXT->fptr, "  pupilCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilCounts[i]);
        
                fprintf(CONTEXT->fptr, "  expelledPupilCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->expelledPupilCounts[i]);
        
                fprintf(CONTEXT->fptr, "  systemTeacherCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->systemTeacherCounts[i]);
        
                fprintf(CONTEXT->fptr, "  professionalStaffCounts:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->professionalStaffCounts[i]);
        
                fprintf(CONTEXT->fptr, "  professionalsInMoreThanOneSchool:\n");
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->professionalsInMoreThanOneSchool[i]);
        
                fprintf(CONTEXT->fptr, "  pupilsInAnotherSystemCounts:\n");
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsInAnotherSystemCounts[i]);
        
                fprintf(CONTEXT->fptr, "  pupilsInNonPublicSchoolsCounts:\n");
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsInNonPublicSchoolsCounts[i]);
        
                fprintf(CONTEXT->fptr, "  pupilsSchoolAgeNotInSchoolCounts:\n");
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsSchoolAgeNotInSchoolCounts[i]);
        
                fprintf(CONTEXT->fptr, "  pupilsNonResidentCounts:\n");
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsNonResidentCounts[i]);
        
                fprintf(CONTEXT->fptr, "  pupilsResidentCounts:\n");
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->fptr, "    \"%s\": %u\n", nara_ethnicity_labels[i], district->pupilsResidentCounts[i]);
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
                        "%u,",
                        district->schoolSystemCode,
                        district->oeCode1970,
                        srgCode,
                        systemName,
                        systemStreetAddr,
                        systemCity,
                        systemCounty,
                        systemState,
                        systemZipCode,
                        systemAdminOfficer,
                        district->numSchoolCampusForms,
                        district->nonResidentPupils,
                        district->residentPupils,
                        district->residentPupilsInOtherSystem,
                        district->residentPupilsInNonpublicSchools,
                        district->residentSchoolAgeNotInSchool,
                        district->bilingualInstruction,
                        district->bilingualTeacherCount,
                        district->bilingualPupilCount,
                        district->bilingualInstructionMaterials,
                        district->newSchoolProperty,
                        district->newSchoolConstruction,
                        district->newSchoolCapacity,
                        district->newSchoolGreaterMinorityComposition,
                        district->year,
                        district->stateCode,
                        district->assurance,
                        district->litigationCode,
                        district->selectionCode,
                        district->samplingWeight
                    );
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->expelledPupilCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->systemTeacherCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->professionalStaffCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_max; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->professionalsInMoreThanOneSchool[i]);
        
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsInAnotherSystemCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsInNonPublicSchoolsCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsSchoolAgeNotInSchoolCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_total; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsNonResidentCounts[i]);
        
                for ( i = 0; i < nara_ethnicity_total - 1; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->pupilsResidentCounts[i]);
                fprintf(CONTEXT->districtFptr, "%u\n", district->pupilsResidentCounts[i]);
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
    