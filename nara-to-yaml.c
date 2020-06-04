/*
* nara-to-yaml
*
* Program to read records from a NARA compressed data archive file
* and output the records as YAML dictionaries.
*
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>

#include "iconv.h"

#include "nara_state_header.h"
#include "nara_record_header.h"
#include "nara_record.h"

/**/

static struct option cliOptions[] = {
        { "help",           no_argument,            0, 'h' },
        { "output",         required_argument,      0, 'o' },
        { NULL, 0, 0, 0 }
    };
const char *cliOptionsStr = "ho:";

/**/

void
usage(
    const char      *exe
)
{
    printf(
            "usage:\n\n"
            "    %s {options} <nara-file-1> {<nara-file-2> ..}\n\n"
            "  options:\n\n"
            "    -h/--help                      display this help info\n"
            "    -o/--output <output-spec>      select the format and file(s) to which output is\n"
            "                                   written\n"
            "\n"
            "    <output-spec> = <format>:<format-arguments>\n"
            "    <format> = yaml | csv\n"
            "    <format-arguments> =\n"
            "        yaml:   <filename>\n"
            "        csv:    <filename>:<filename>:<filename>\n"
            "    <filename> = <path-to-a-file> | - (meaning stdout) | <empty> (no output)\n"
            "    <empty> = \"\"\n"
            "\n"
            "    YAML outputs to a single file, whereas CSV outputs to three separate files for\n"
            "    each record type (first is district filename, second is school filename, third\n"
            "    is classroom file name)\n"
            "\n"
            "    The default output specification is \"yaml:-\" to output YAML to stdout.\n"
            "\n",
            exe
        );
}

/**/

int
main(
    int                     argc,
    char* const             argv[]
)
{
    int                     argi = 1, optc;
    int                     rc = 0;
    int                     sawStdin = 0;
    
    nara_export_context_t   exportContext = NULL;
    const char              *outputSpec = "yaml:-";
    
    if ( argc < 2 ) {
        usage(argv[0]);
        exit(EINVAL);
    }
    
    while ( (optc = getopt_long(argc, argv, cliOptionsStr, cliOptions, NULL)) != -1 ) {
        switch ( optc ) {
        
            case 'h':
                usage(argv[0]);
                exit(0);
            
            case 'o':
                outputSpec = optarg;
                break;
        
        }
    }
    argi = optind;
    if ( argi == argc ) {
        fprintf(stderr, "ERROR:  no NARA files provided\n");
        usage(argv[0]);
        exit(EINVAL);
    }
    
    nara_endian_init();
    
    /*
     * Initialize export context:
     */
    exportContext = nara_export_init(outputSpec);
    if ( ! exportContext ) exit(EINVAL);
    
    while ( (rc == 0) && (argi < argc) ) {
        FILE    *fptr;
        int     shouldFClose = 1;
        
        if ( strcmp(argv[argi], "-") == 0 ) {
            if ( sawStdin ) {
                fprintf(stderr, "ERROR:  saw stdin ('-') file multiple times!\n");
                exit(EINVAL);
            }
            fptr = stdin;
            sawStdin = 1;
            shouldFClose = 0;
        } else {
            fptr = fopen(argv[argi], "r");
        }
        
        if ( fptr ) {
            nara_state_header_t         stateHeader;
            uint64_t                    stateRecordCount = 0, totalRecordCount = 0, nextStateRecordOffset = 0;
            size_t                      bytesRead;
            
            /* Loop over variable-length state records in the file: */
            while ( (rc == 0) && ((bytesRead = fread(&stateHeader, sizeof(stateHeader), 1, fptr)) == 1) ) {
                nara_record_header_t    recordHeader;
                uint64_t                nextRecordOffset;
                
                /* Process the header (endian swap, etc.): */
                nara_state_header_process(&stateHeader);
                
                /*
                 * Calculate offset of the first record for the state and the next state in the file:
                 */
                ++stateRecordCount;
                nextRecordOffset = nextStateRecordOffset + sizeof(stateHeader);
                nextStateRecordOffset += stateHeader.recordLength;
                
                /* Now handle each one of this state's records: */
                while ( (bytesRead = fread(&recordHeader, sizeof(recordHeader), 1, fptr)) == 1 ) {
                    nara_record_t       *nextRecord;
                    size_t              wantToRead;
                    
                    /* Process the header (endian swap, etc.): */
                    nara_record_header_process(&recordHeader);
                    
                    /*
                     * Increase the total record count and calculate how large this record is and
                     * where the next record will occur:
                     */
                    ++totalRecordCount;
                    wantToRead = recordHeader.recordLength - sizeof(recordHeader);
                    nextRecordOffset += recordHeader.recordLength;
                    
                    /* Read the record: */
                    nextRecord = nara_record_read(fptr, wantToRead);
                    
                    /* Read the record type: */
                    if ( nextRecord ) {
                        nara_record_export(exportContext, nextRecord);
                        nextRecord = nara_record_destroy(nextRecord);
                    } else {
                        printf("ERROR:  unable to read record\n");
                        rc = 5;
                    }
                    
                    /*
                     * If we're now past the end of the current chunk move on to the
                     * next one:
                     */
                    if ( nextRecordOffset >= nextStateRecordOffset ) break;
                }
                if ( nextRecordOffset != nextStateRecordOffset ) {
                    printf("ERROR:  end of secondary records extends beyond primary record bounds\n");
                    rc = 2;
                }
            }
            
            if ( shouldFClose ) fclose(fptr);
        }
        argi++;
    }
    
    nara_export_destroy(exportContext);
    
    return rc;
}
