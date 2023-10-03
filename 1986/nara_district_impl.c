/*
 * nara_district_impl
 *
 * The NARA "district" record implementation.
 *
 */

#include "nara_district.h"


typedef union {
    nara_district_t    break_out;
    struct {
        uint32_t        f1[3];
        char            systemName[32];
        char            systemStreetAddress[32];
        char            systemCounty[16];
        char            systemCity[16];
        char            systemStateAbbrev[4];
        char            systemZipCode[8];
        uint32_t        f2[300];
        float           f3;
        uint32_t        f4;
        float           f5;
        uint32_t        f6;
        uint32_t        unused2[154];
        uint32_t        f7[114];
        /*
         * We won't bother byte-swapping all these
         */
        uint32_t        unused3[98];
    } compact;
} nara_district_internal_t;

/**/

int
__nara_record_is_type_district(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return ( (byteSize == NARA_1986_RECORD_SIZE) && (nara_be_to_host_32(theRecord->recordType) == nara_record_type_district) );
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
    
    district->compact.f4 = nara_be_to_host_32(district->compact.f4);
    
    district->compact.f5 = nara_be_to_host_float(district->compact.f5);
    
    district->compact.f6 = nara_be_to_host_32(district->compact.f6);
    
    iMax = sizeof(district->compact.f7) / sizeof(uint32_t);
    for ( i = 0; i < iMax; i ++ ) district->compact.f7[i] = nara_be_to_host_32(district->compact.f7[i]);
    
    TRANSCODE(district, systemName, "district name");
    TRANSCODE(district, systemStreetAddress, "district street address");
    TRANSCODE(district, systemCounty, "district county");
    TRANSCODE(district, systemCity, "district city");
    TRANSCODE(district, systemStateAbbrev, "state abbreviation");
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
                        "systemStreetAddress,"
                        "systemCounty,"
                        "systemCity,"
                        "systemStateAbbrev,"
                        "systemZipCode,"
                        "numSchoolsInSchoolSystem,"
                        "isCourtOrderYesFederal,"
                        "isCourtOrderYesState,"
                        "isCourtOrderNo,"
                        "childrenAwaitingInitEval,"
                        "childrenRequireSpecialEd,"
                        "childrenReceiveSpecialEdInDistrict,"
                        "childrenReceiveSpecialEdNonDistrict,"
                        "sampleWeight,"
                        "isSubSampledDistrict,"
                        "subSampledWeight,"
                        "isSubSampledSchool,"
                    );
                
                for ( i = 1; i <= 32; i++ )
                    fprintf(CONTEXT->districtFptr, "errorBitArray_%d,", i);
                    
                for ( i = 1; i <= 82; i++ )
                    fprintf(CONTEXT->districtFptr, "conditionCode_%d,", i);
                fprintf(CONTEXT->districtFptr, "conditionCode_%d\n", i);
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
    LOCAL_STR_DECL(systemStreetAddress, sizeof(district->systemStreetAddress));
    LOCAL_STR_DECL(systemCounty, sizeof(district->systemCounty));
    LOCAL_STR_DECL(systemCity, sizeof(district->systemCity));
    LOCAL_STR_DECL(systemStateAbbrev, sizeof(district->systemStateAbbrev));
    LOCAL_STR_DECL(systemZipCode, sizeof(district->systemZipCode));
    
    LOCAL_STR_FILL(systemName, sizeof(district->systemName), district->systemName);
    LOCAL_STR_FILL(systemStreetAddress, sizeof(district->systemStreetAddress), district->systemStreetAddress);
    LOCAL_STR_FILL(systemCounty, sizeof(district->systemCounty), district->systemCounty);
    LOCAL_STR_FILL(systemCity, sizeof(district->systemCity), district->systemCity);
    LOCAL_STR_FILL(systemStateAbbrev, sizeof(district->systemStateAbbrev), district->systemStateAbbrev);
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
                        "  systemStreetAddress: %s\n"
                        "  systemCounty: %s\n"
                        "  systemCity: %s\n"
                        "  systemStatAbbrev: %s\n"
                        "  systemZipCode: %s\n"
                        "  numSchoolsInSchoolSystem: %u\n"
                        "  isCourtOrderYesFederal: %u\n"
                        "  isCourtOrderYesState: %u\n"
                        "  isCourtOrderNo: %u\n"
                        "  childrenAwaitingInitEval: %u\n"
                        "  childrenRequireSpecialEd: %u\n"
                        "  childrenReceiveSpecialEdInDistrict: %u\n"
                        "  childrenReceiveSpecialEdNonDistrict: %u\n"
                        "  samplingWeight: %g\n"
                        "  isSubSampledDistrict: %u\n"
                        "  subSampledWeight: %g\n"
                        "  isSubSampledSchool: %u\n",
                        district->systemOECode,
                        district->selectionCode,
                        systemName,
                        systemStreetAddress,
                        systemCounty,
                        systemCity,
                        systemStateAbbrev,
                        systemZipCode,
                        district->numSchoolsInSchoolSystem,
                        district->isCourtOrderYesFederal,
                        district->isCourtOrderYesState,
                        district->isCourtOrderNo,
                        district->childrenAwaitingInitEval,
                        district->childrenRequireSpecialEd,
                        district->childrenReceiveSpecialEdInDistrict,
                        district->childrenReceiveSpecialEdNonDistrict,
                        district->sampleWeight,
                        district->isSubSampledDistrict,
                        district->subSampledWeight,
                        district->isSubSampledSchool
                    );

                fprintf(CONTEXT->fptr, "  errorBitArray:\n");
                for ( i = 0; i < 32; i++ )
                    fprintf(CONTEXT->fptr, "    - %u\n", district->errorBitArray[i]);

                fprintf(CONTEXT->fptr, "  conditionCodes:\n");
                for ( i = 0; i < 82; i++ )
                    fprintf(CONTEXT->fptr, "    - %u\n", district->conditionCodes[i]);
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
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%u,"
                        "%g,"
                        "%u,"
                        "%g,"
                        "%u,",
                        district->systemOECode,
                        district->selectionCode,
                        systemName,
                        systemStreetAddress,
                        systemCounty,
                        systemCity,
                        systemStateAbbrev,
                        systemZipCode,
                        district->numSchoolsInSchoolSystem,
                        district->isCourtOrderYesFederal,
                        district->isCourtOrderYesState,
                        district->isCourtOrderNo,
                        district->childrenAwaitingInitEval,
                        district->childrenRequireSpecialEd,
                        district->childrenReceiveSpecialEdInDistrict,
                        district->childrenReceiveSpecialEdNonDistrict,
                        district->sampleWeight,
                        district->isSubSampledDistrict,
                        district->subSampledWeight,
                        district->isSubSampledSchool
                    );

                for ( i = 0; i < 32; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->errorBitArray[i]);
                    
                for ( i = 0; i < 82; i++ )
                    fprintf(CONTEXT->districtFptr, "%u,", district->conditionCodes[i]);
                fprintf(CONTEXT->districtFptr, "%u\n", district->conditionCodes[i]);
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
    