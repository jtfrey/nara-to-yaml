/*
 * nara_classroom_impl
 *
 * The NARA "classroom" record implementation.
 *
 */

#include "nara_classroom.h"

int
__nara_record_is_type_classroom(
    nara_record_t*      theRecord,
    size_t              byteSize
)
{
    return 0;
}

/**/

nara_record_t*
__nara_record_process_classroom(
    nara_record_t*      theRecord
)
{
    return theRecord;
}

/**/

void
__nara_export_init_classroom(
    nara_export_context_t   exportContext
)
{
}

/**/

void
__nara_export_destroy_classroom(
    nara_export_context_t   exportContext
)
{
}

/**/

void
__nara_record_export_classroom(
    nara_export_context_t   exportContext,
    nara_record_t           *theRecord
)
{
}

/**/

nara_record_t*
__nara_record_destroy_classroom(
    nara_record_t       *theRecord
)
{
    return NULL;
}
    