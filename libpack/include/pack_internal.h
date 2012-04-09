#ifndef __PACK_INTERNAL_H__
#define __PACK_INTERNAL_H__

#include <stdarg.h>
#include <stdlib.h>

#include "libpack.h"

enum packtemplate_type
{
  // Portable-sized integers
  PACK_UNSIGNED,

  // Platform-sized integers
  PACK_CHAR,
  PACK_SHORT,
  PACK_INT,
  PACK_LONG,
};

enum packtemplate_endian
{
  PACK_ENDIAN_HOST,
  PACK_ENDIAN_LITTLE,
  PACK_ENDIAN_BIG,
};

enum packtemplate_arrtype
{
  PACK_ARRAY_NONE,
  PACK_ARRAY_FIXED,
  PACK_ARRAY_DYNAMIC_ARGS,
  PACK_ARRAY_DYNAMIC_STREAM,
};

struct packtemplate_item
{
  enum packtemplate_type type;
  enum packtemplate_endian endian;
  enum packtemplate_arrtype array;
  int bitdepth;
  int count;
  int arrsize; // Valid only for array==PACK_ARRAY_FIXED
  enum packtemplate_type arrsize_type; // Valid only for array==PACK_ARRAY_DYNAMIC_STREAM
  int arrsize_bitdepth;                // ""
};

struct packtemplate_list
{
  struct packtemplate_item item;
  struct packtemplate_list *next;
};

struct packtemplate_list *parse_template(const char *tmpl);
void free_template(struct packtemplate_list *list);

typedef struct
{
  size_t (*read) (PackStream *stream,       void *buffer, size_t n);
  size_t (*write)(PackStream *stream, const void *buffer, size_t n);

  size_t (*remaining)(PackStream *stream);
} PackStreamVtable;

struct _PackStream
{
  PackStreamVtable *vtable;
  size_t readpos, writepos;
};

#endif
