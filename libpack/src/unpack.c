#include "pack_internal.h"

#include "macros.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t vunpack(PackStream *stream, const char *tmpl, va_list args)
{
  struct packtemplate_list *list = parse_template(tmpl);
  if(!list)
    return -1;

  size_t start_pos = stream->readpos;
#define READ(b,n) \
  if((*stream->vtable->read)(stream, (b), (n)) == 0) \
    goto stop_now;

  for(struct packtemplate_list *l = list; l; l = l->next) {
    for(int c = 0; c < l->item.count; c++) {
      // arrsize is the number of ITEMS to unpack from the stream
      // buffsize is the number of ITEMS the storage buffer can hold
      int arrsize;
      int arrsizevalid = 1;
      int buffsize;
      void *array;
      void *realarray;
      enum packtemplate_type type = l->item.type;
      int bitdepth = l->item.bitdepth;

      switch(l->item.array) {
      case PACK_ARRAY_NONE:
        array = va_arg(args, void*);
        arrsize = 1;
        buffsize = 1;
        break;
      case PACK_ARRAY_FIXED:
        array = va_arg(args, void*);
        arrsize = l->item.arrsize;
        buffsize = arrsize;
        break;
      case PACK_ARRAY_DYNAMIC_ARGS:
        array = va_arg(args, void*);
        arrsize = va_arg(args, int);
        buffsize = arrsize;
        break;
      case PACK_ARRAY_DYNAMIC_STREAM:
        // In this case; get the for loop below to read the size for us
        realarray = va_arg(args, void*);
        array = va_arg(args, void*);
        arrsize = 1;
        buffsize = 1;
        arrsizevalid = 0;
        type = l->item.arrsize_type;
        bitdepth = l->item.arrsize_bitdepth;
        break;
      }

      int index;
      for(index = 0; index < arrsize; index++) {
        switch(type) {
// To keep the following code cases small, we'll use a macro for each case
#define PRE_VAR(T) \
  { \
    T dummy; \
    T *valp = (index < buffsize) ? ((T*)array) + index : &dummy; \
    if(!arrsizevalid) \
      buffsize = *valp;
#define POST_VAR \
    if(!arrsizevalid) { \
      arrsize = *valp; \
      if(arrsize > buffsize) \
        *valp = buffsize; \
    } \
  }

        case PACK_CHAR:
          PRE_VAR(char);
          READ(valp, sizeof(*valp));
          POST_VAR;
          break;
        case PACK_SHORT:
          PRE_VAR(short);
          READ(valp, sizeof(*valp));
          POST_VAR;
          break;
        case PACK_INT:
          PRE_VAR(int);
          READ(valp, sizeof(*valp));
          POST_VAR;
          break;
        case PACK_LONG:
          PRE_VAR(long);
          READ(valp, sizeof(*valp));
          POST_VAR;
          break;
        case PACK_UNSIGNED:
          switch(bitdepth) {
          case 8:
            PRE_VAR(uint8_t);
            READ(valp, sizeof(*valp));
            POST_VAR;
            break;
          case 16:
            PRE_VAR(uint16_t);
            if(l->item.endian == PACK_ENDIAN_HOST) {
              READ(valp, sizeof(*valp));
            }
            else {
              unsigned char b[2];
              READ(b, 2);
              BYTEARR_TO_U16(b, *valp, l->item.endian);
            }
            POST_VAR;
            break;
          case 32:
            PRE_VAR(uint32_t);
            if(l->item.endian == PACK_ENDIAN_HOST) {
              READ(valp, sizeof(*valp));
            }
            else {
              unsigned char b[4];
              READ(b, 4);
              BYTEARR_TO_U32(b, *valp, l->item.endian);
            }
            POST_VAR;
            break;
          case 64:
            PRE_VAR(uint64_t);
            if(l->item.endian == PACK_ENDIAN_HOST) {
              READ(valp, sizeof(*valp));
            }
            else {
              unsigned char b[8];
              READ(b, 8);
              BYTEARR_TO_U64(b, *valp, l->item.endian);
            }
            POST_VAR;
            break;
          }
          break;
        default:
          ASSERT_UNREACHABLE;
        }

        if(!arrsizevalid) {
          // We've just read the size from the stream. Reset variables for the
          // real array reading and restart the loop
          arrsizevalid = 1;
          array = realarray;
          type = l->item.type;
          bitdepth = l->item.bitdepth;
          index = -1; // so the next ++ will set it back to 0
        }
      }
    }
  }

stop_now:
  free_template(list);

  return stream->readpos - start_pos;
}

size_t unpack(PackStream *stream, const char *tmpl, ...)
{
  va_list args;
  va_start(args, tmpl);

  size_t ret = vunpack(stream, tmpl, args);

  va_end(args);
  return ret;
}
