#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>

#include "skiplist.h"
#include "hprof_backtrace.h"
#include "swab.h"

#include "libpack.h"

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

int __true_cmp(id lhs, id rhs) {
  if (lhs > rhs) {
    return SKIPLIST_CMP_GT;
  } else if (lhs == rhs) {
    return SKIPLIST_CMP_EQ;
  } else {
    return SKIPLIST_CMP_LT;
  }
}

skiplist_cmp_t stringtable_cmp(const void *list_value, const void *user_value) {
    const hprof_utf8 *l = list_value;
    const hprof_utf8 *v = user_value;
    return __true_cmp(l->id, v->id);
}

skiplist_cmp_t stringtable_search(const void *list_value, const void *search_value) {
    const hprof_utf8 *l = list_value;
    const id to_find = *(id*) search_value;
    return __true_cmp(l->id, to_find);
}

void stringtable_destroy(void *value) {
    free(value);
}

/**
 * Caller is responsible for free() on the returned pointer
 * returns NULL is a missing id, or failure to craft a string (malloc)
 */
char* read_string(id str_id, skiplist_t *string_table, FILE *fp) {
    const hprof_utf8 *utf8 = skiplist_search(string_table, &str_id);
    if(utf8 == NULL) {
        // NOT FOUND
        return NULL;
    }

    // Filthy trick, use calloc with an extra char and we get the
    // null terminator for free, its probably safer from the buffer
    // overrun perspective to use asnprintf
    char *to_return = calloc(utf8->num_chars + 1, sizeof(char));
    if (to_return == NULL) {
        return NULL;
    }

    long curr_pos = ftell(fp);
    fseek(fp, utf8->hprof_offset, SEEK_SET);
    size_t num_read = fread(to_return, sizeof(char), utf8->num_chars, fp);
    if (num_read != utf8->num_chars) {
        return NULL;
    }
    fseek(fp, curr_pos, SEEK_SET);

    return to_return;
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

    // The string table is a skip list that should give us
    // O(n log n) insertion and O(log n) lookup, This is not as 
    // good as a malloc'd array, but does not suffer from the
    // out of order issues that the string id's present us with
    skiplist_t *string_table = malloc(sizeof(skiplist_t));
    skiplist_init(string_table);
    skiplist_set_cmp_fn(string_table, stringtable_cmp);
    skiplist_set_search_fn(string_table, stringtable_search);
    skiplist_set_destroy_fn(string_table, stringtable_destroy);

    while(1) {
        if (feof(hprof_fp)) {
            break;
        }

        //read_record_header(record_header, hprof_fp);
        funpack(hprof_fp, "bd>d>",
            &record_header->tag,
            &record_header->profiling_ms,
            &record_header->remaining_bytes);

        // libpack attempt


        //printf("pos=%lu\n", ftell(hprof_fp));
        //printf("Tag=%ju\n", (uintmax_t) record_header->tag);
        switch(record_header->tag) {
            case UTF8:
                printf("UTF8\n");
                hprof_utf8 *utf8 = malloc(sizeof(hprof_utf8));
                fread(&utf8->id, sizeof(id), 1, hprof_fp);
                int bytes_remain = be32_to_native(record_header->remaining_bytes) - sizeof(id);

                // Awesome, hprof strings are utf8, this is good ! the only established
                // C type is char and its a byte wide :D
                utf8->num_chars = bytes_remain / sizeof(char);
                utf8->hprof_offset = ftell(hprof_fp);

                skiplist_insert(string_table, utf8);

                fseek(hprof_fp, bytes_remain, SEEK_CUR);

                /*
                char* name = calloc(num_chars + 1, sizeof(char));
                fread(name, sizeof(char), num_chars, hprof_fp);
                //printf("%s\n", name);
                printf("%ju\n", (uintmax_t) be64_to_native(id));
                free(name);
                */
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
                
                // This is bad !!! If it is slow think about doing two complete passes to 
                // make all the I/O as sequential as possible
                // This implementation is going to jump all over the hprof file
                long curr_pos = ftell(hprof_fp);
                id ids[4];
                fread(&ids, sizeof(id), 4, hprof_fp);

                char *method_name = read_string(ids[1], string_table, hprof_fp);
                char *method_sig = read_string(ids[2], string_table, hprof_fp);
                char *source_file = read_string(ids[3], string_table, hprof_fp);

                method_name = (method_name == NULL) ? "<UNKNOWN_METHOD>"     : method_name;
                method_sig  = (method_sig  == NULL) ? "<UNKNOWN_METHOD_SIG>" : method_sig;
                source_file = (source_file == NULL) ? "<UNKNOWN_SOURCE>"     : source_file;

                // SOMETIMES NULL
                //printf("%s\n", read_string(ids[3], string_table, hprof_fp));
                
                u4 class_serial_num;
                fread(&class_serial_num, sizeof(u4), 1, hprof_fp);
                i4 line_num;
                fread(&line_num, sizeof(i4), 1, hprof_fp);
                line_num = be32_to_native(line_num);

                switch(line_num) {
                    case -1:
                        printf("Class (TODO String table lookup) %ju\n \tMethod=%s\n\tSource=%s @ <UNKNOWN>\n",
                                (uintmax_t) class_serial_num, method_name, source_file);
                        break;
                    case -2:
                        printf("Class (TODO String table lookup) %ju\n \tMethod=%s\n\tSource=%s @ <COMPILED_METHOD>\n",
                                (uintmax_t) class_serial_num, method_name, source_file);
                        break;
                    case -3:
                        printf("Class (TODO String table lookup) %ju\n \tMethod=%s\n\tSource=%s @ <NATIVE_METHOD>\n",
                                (uintmax_t) class_serial_num, method_name, source_file);
                        break;
                    default:
                        printf("Class (TODO String table lookup) %ju\n \tMethod=%s\n\tSource=%s @ %ji\n",
                                (uintmax_t) class_serial_num, method_name, source_file, (intmax_t) line_num);
                        break;
                }

                break;
            case TRACE:
                printf("TRACE\n");
                hprof_stacktrace* stacktrace = calloc(1, sizeof(hprof_stacktrace));

                fread(&stacktrace->serial_number, sizeof(u4), 1, hprof_fp);
                fread(&stacktrace->thread_number, sizeof(u4), 1, hprof_fp);
                fread(&stacktrace->number_frames, sizeof(u4), 1, hprof_fp);

                printf("StackTrace serial_num=%ju "
                       "thread_num=%ju num_frames=%ju\n",
                       (uintmax_t) be32_to_native(stacktrace->serial_number),
                       (uintmax_t) be32_to_native(stacktrace->thread_number),
                       (uintmax_t) be32_to_native(stacktrace->number_frames));

                for (int i=0; i < be32_to_native(stacktrace->number_frames); i++) {
                    id frame_id;
                    fread(&frame_id, sizeof(id), 1, hprof_fp);
                    printf("FrameID=%ju\n", (uintmax_t) be64_to_native(frame_id));
                }

                free(stacktrace);

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

    skiplist_destroy(string_table);
    free(string_table);

    fclose(hprof_fp);
    return 0;
}
