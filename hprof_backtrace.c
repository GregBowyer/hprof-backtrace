#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>

#include "hprof_backtrace.h"
#include "swab.h"

void die(int check, char* message) {
    if (check) {
        printf("%s\n", message);
        exit(1);
    }
}

int read_record_header(hprof_record_header* record_header, FILE* hprof_fp) {
    int count = fread(&record_header->tag, sizeof(u1), 1, hprof_fp);
    count += fread(&record_header->profiling_ms, sizeof(u4), 1, hprof_fp);
    count += fread(&record_header->remaining_bytes, sizeof(u4), 1, hprof_fp);
    return !(count == 3);
}

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s HPROF_FILE\n", argv[0]);
        exit(1);
    }

    FILE* hprof_fp = fopen(argv[1], "r");
    die(hprof_fp == NULL, "File error");

    hprof_header* header;
    char version[19];
    u4 ident_size;

    int count = fread(version, sizeof(version), 1, hprof_fp);
    die(count != 1, "Unable to read hprof header");

    count = fread(&ident_size, sizeof(u4), 1, hprof_fp);
    die(count != 1, "Unable to read hprof header");

    int success = fseek(hprof_fp, sizeof(u8), SEEK_CUR);
    die(success != 0, "Could not seek forward in the file");

    printf("Header: version=%s identsize=%ju\n",
            version,
            (uintmax_t) be32_to_native(ident_size));

    // For now just reuse it
    hprof_record_header* record_header = calloc(1, sizeof(hprof_record_header));

    while(1) {
        if (feof(hprof_fp)) {
            break;
        }

        read_record_header(record_header, hprof_fp);

        switch(record_header->tag) {
            case UTF8:
                printf("UTF8\n");
            case LOAD_CLASS:
                printf("LOAD_CLASS\n");
            case UNLOAD_CLASS:
                printf("UNLOAD_CLASS\n");
            case FRAME:
                printf("FRAME\n");
            case TRACE:
                printf("TRACE\n");
            case ALLOC_SITES:
                printf("ALLOC_SITES\n");
            case HEAP_SUMMARY:
                printf("HEAP_SUMMARY\n");
            case START_THREAD:
                printf("START_THREAD\n");
            case END_THREAD:
                printf("END_THREAD\n");
            case HEAP_DUMP:
                printf("HEAP_DUMP\n");
            case CPU_SAMPLES:
                printf("CPU_SAMPLES\n");
            case CONTROL_SETTINGS:
                printf("CONTROL_SETTINGS\n");
            case HEAP_DUMP_SEGMENT:
                printf("HEAP_DUMP_SEGMENT\n");
            case HEAP_DUMP_END:
                printf("HEAP_DUMP_END\n");
            default:
                fseek(hprof_fp, record_header->remaining_bytes, SEEK_CUR);
                break;
        }
    }

    fclose(hprof_fp);
    return 0;
}
