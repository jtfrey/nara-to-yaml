/*
 * nara_record_impl
 *
 * Baseline shared API for implementation of each record type.
 *
 */

#ifndef __NARA_RECORD_IMPL_H__
#define __NARA_RECORD_IMPL_H__

#include "nara_base.h"

#ifdef HAVE_EBCDIC_ENCODING
#   include "nara_ebcdic.h"
#endif

/*
 * The LOCAL_STR_DECL() macro declares a local char array with the given name (N)
 * and length (L) plus a NUL terminator.
 *
 * Used to quickly turn strings in records into C strings for output.
 *
 */
#define LOCAL_STR_DECL(N, L)    char     N[(L) + 1]

/*
 * The LOCAL_STR_FILL() macro copies the string inside a record into a locally-declared
 * C string.  A NUL terminator is added and any trailing whitespace is removed.  Double
 * quotes inside the remainder of the string are turned into single quotes to avoid
 * parse errors with the generated YAML.
 *
 * Used to quickly turn strings in records into C strings for output.
 *
 */
#define LOCAL_STR_FILL(N, L, P) memcpy((char*)N, (char*)P, (L)); N[(L)] = '\0'; { int N_idx = (L) - 1; while ( N_idx >= 0 ) { if (N[N_idx] && ! isspace(N[N_idx])) break; N[N_idx--] = '\0'; } while ( N_idx >= 0 ) { if (N[N_idx] == '"') N[N_idx] = '\''; N_idx--; }}

/*
 * Generic macro used to drive the EBCDIC-to-ASCII conversion of strings inside the
 * NARA records.
 */
#ifdef HAVE_EBCDIC_ENCODING
#   define TRANSCODE(T, N, F) nara_ebcdic_to_ascii_inplace(T->break_out.N, 0)
#else
#   define TRANSCODE(T, N, F)
#endif

typedef int (*nara_record_is_type_fn)(nara_record_t *theRecord, size_t byteSize);

typedef nara_record_t* (*nara_record_process_fn)(nara_record_t *theRecord);

enum {
    nara_export_format_yaml = 0,
    nara_export_format_csv = 1,
    nara_export_format_max
};

typedef struct {
    unsigned int    format;
} nara_export_context_base_t;

typedef struct {
    nara_export_context_base_t  base;
    FILE                        *fptr;
} nara_export_context_yaml_t;

typedef struct {
    nara_export_context_base_t  base;
    FILE                        *districtFptr;
    FILE                        *schoolFptr;
    FILE                        *classroomFptr;
} nara_export_context_csv_t;

typedef void (*nara_export_init_fn)(nara_export_context_t exportContext);
typedef void (*nara_record_export_fn)(nara_export_context_t exportContext, nara_record_t *theRecord);
typedef void (*nara_export_destroy_fn)(nara_export_context_t exportContext);

typedef nara_record_t* (*nara_record_destroy_fn)(nara_record_t *theRecord);

#endif /* __NARA_RECORD_IMPL_H__ */
