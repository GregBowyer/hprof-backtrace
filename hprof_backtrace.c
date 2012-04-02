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

void decode_gc_tag(hprof_gc_tag gc_tag) {
    switch(gc_tag) {
        case ROOT_UNKNOWN:
            printf("ROOT_UNKNOWN\n");
            break;
        case ROOT_JNI_GLOBAL:
            printf("ROOT_JNI_GLOBAL\n");
            break;
        case ROOT_JNI_LOCAL:
            printf("ROOT_JNI_LOCAL\n");
            break;
        case ROOT_JAVA_FRAME:
            printf("ROOT_JAVA_FRAME\n");
            break;
        case ROOT_NATIVE_STACK:
            printf("ROOT_NATIVE_STACK\n");
            break;
        case ROOT_STICKY_CLASS:
            printf("ROOT_STICKY_CLASS\n");
            break;
        case ROOT_THREAD_BLOCK:
            printf("ROOT_THREAD_BLOCK\n");
            break;
        case ROOT_MONITOR_USED:
            printf("ROOT_MONITOR_USED\n");
            break;
        case ROOT_THREAD_OBJ:
            printf("ROOT_THREAD_OBJ\n");
            break;
        case CLASS_DUMP:
            printf("CLASS_DUMP\n");
            break;
        case INSTANCE_DUMP:
            printf("INSTANCE_DUMP\n");
            break;
        case OBJ_ARRAY_DUMP:
            printf("OBJ_ARRAY_DUMP\n");
            break;
        case PRIM_ARRAY_DUMP:
            printf("PRIM_ARRAY_DUMP\n");
            break;
        default:
            printf("Huh\n");
    }
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
    hprof_heap_summary* heap_summary = calloc(1, sizeof(hprof_heap_summary));

    while(1) {
        if (feof(hprof_fp)) {
            break;
        }

        read_record_header(record_header, hprof_fp);

        printf("pos=%lu\n", ftell(hprof_fp));
        printf("Tag=%ju\n", (uintmax_t) record_header->tag);
        switch(record_header->tag) {
            case UTF8:
                printf("UTF8\n");
                u8 id;
                fread(&id, sizeof(u8), 1, hprof_fp);
                size_t num_chars = 
                    be32_to_native(record_header->remaining_bytes) - sizeof(u8) / sizeof(char);
                char* name = calloc(num_chars + 1, sizeof(char));
                fread(name, sizeof(char), num_chars, hprof_fp);
                printf("%s\n", name);
                free(name);
                //fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case LOAD_CLASS:
                printf("LOAD_CLASS\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case UNLOAD_CLASS:
                printf("UNLOAD_CLASS\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case FRAME:
                printf("FRAME\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case TRACE:
                printf("TRACE\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case ALLOC_SITES:
                printf("ALLOC_SITES\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case HEAP_SUMMARY:
                printf("HEAP SUMMARY\n");
                fread(&heap_summary->live_bytes, sizeof(u4), 1, hprof_fp);
                fread(&heap_summary->live_instances, sizeof(u4), 1, hprof_fp);
                fread(&heap_summary->allocated_bytes, sizeof(u8), 1, hprof_fp);
                fread(&heap_summary->allocated_instances, sizeof(u8), 1, hprof_fp);
                printf("Heap summary:\n\tReachable bytes=%ju\n\t"
                       "Reachable instances=%ju\n\t"
                       "Allocated bytes=%ju\n\t"
                       "Allocated instances=%ju\n",
                       (uintmax_t) be32_to_native(heap_summary->live_bytes),
                       (uintmax_t) be32_to_native(heap_summary->live_instances),
                       (uintmax_t) be64_to_native(heap_summary->allocated_bytes),
                       (uintmax_t) be64_to_native(heap_summary->allocated_instances));
                break;
            case START_THREAD:
                printf("START_THREAD\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case END_THREAD:
                printf("END_THREAD\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case HEAP_DUMP:
                printf("HEAP_DUMP\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case CPU_SAMPLES:
                printf("CPU_SAMPLES\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case CONTROL_SETTINGS:
                printf("CONTROL_SETTINGS\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case HEAP_DUMP_SEGMENT:
                printf("HEAP_DUMP_SEGMENT\n");
                /*
                u1* gc_tag = calloc(1, sizeof(u1));
                fread(&gc_tag, sizeof(u1), 1, hprof_fp);
                decode_gc_tag(*gc_tag);
                */
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                break;
            case HEAP_DUMP_END:
                printf("HEAP_DUMP_END\n");
                fseek(hprof_fp, be32_to_native(record_header->remaining_bytes), SEEK_CUR);
                exit(1);
                break;
            default:
                printf("BANG\n");
                exit(1);
                break;
        }

        //printf("%lu\n", ftell(hprof_fp));
    }

    fclose(hprof_fp);
    return 0;
}
