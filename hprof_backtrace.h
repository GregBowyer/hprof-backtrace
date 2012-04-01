#ifndef __hprof_backtrace_h__
#define __hprof_backtrace_h__

#include <stdint.h>

// This is what the documentation refers to
// *Endieness unknown, assume native*
typedef uint8_t u1;
typedef uint32_t u4;
typedef uint64_t u8;

typedef enum {
    UTF8                    = 0x01,
    LOAD_CLASS              = 0x02,
    UNLOAD_CLASS            = 0x03,
    FRAME                   = 0x04,
    TRACE                   = 0x05,
    ALLOC_SITES             = 0x06,
    HEAP_SUMMARY            = 0x07,
    START_THREAD            = 0x0A,
    END_THREAD              = 0x0B,
    HEAP_DUMP               = 0x0C,
    HEAP_DUMP_SEGMENT       = 0x1C, /* 1.0.2 only */
    HEAP_DUMP_END           = 0x2C, /* 1.0.2 only */
    CPU_SAMPLES             = 0x0D,
    CONTROL_SETTINGS        = 0x0E
} hprof_tag;

typedef enum {
    GC_ROOT_UNKNOWN       = 0xFF,
    GC_ROOT_JNI_GLOBAL    = 0x01,
    GC_ROOT_JNI_LOCAL     = 0x02,
    GC_ROOT_JAVA_FRAME    = 0x03,
    GC_ROOT_NATIVE_STACK  = 0x04,
    GC_ROOT_STICKY_CLASS  = 0x05,
    GC_ROOT_THREAD_BLOCK  = 0x06,
    GC_ROOT_MONITOR_USED  = 0x07,
    GC_ROOT_THREAD_OBJ    = 0x08,
    GC_CLASS_DUMP         = 0x20,
    GC_INSTANCE_DUMP      = 0x21,
    GC_OBJ_ARRAY_DUMP     = 0x22,
    GC_PRIM_ARRAY_DUMP    = 0x23
} hprof_gc_tag;

typedef enum {
    ARRAY_OBJECT      = 1,
    NORMAL_OBJECT     = 2,
    BOOLEAN           = 4,
    CHAR              = 5,
    FLOAT             = 6,
    DOUBLE            = 7,
    BYTE              = 8,
    SHORT             = 9,
    INT               = 10,
    LONG              = 11
} hprof_object_type;

typedef struct {
    u4 ident_size;
    u4 high_word;
    u4 low_word;
} hprof_header;

typedef struct {
    hprof_tag tag;
    u4 profiling_ms;
    u4 remaining_bytes;
    //[u1]* body of record
} hprof_record_header;

#endif
