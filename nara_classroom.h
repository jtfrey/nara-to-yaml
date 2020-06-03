/*
 * nara_classroom
 *
 * The NARA "classroom" type record as a data structure.  This is the public
 * interface to the record; there's also in internal one in nara_classroom_impl.c
 * that's used to process byte-swapping more efficiently.
 *
 */

#ifndef __NARA_CLASSROOM_H__
#define __NARA_CLASSROOM_H__

#include "nara_school.h"

typedef struct {
    uint32_t        recordType;
    uint32_t        schoolSystemCode;
    uint32_t        schoolCode;
    uint32_t        gradeLevel;
    uint32_t        classroomCode;
    uint32_t        pupilCounts[nara_ethnicity_total];
} nara_classroom_t;

#endif /* __NARA_CLASSROOM_H__ */
