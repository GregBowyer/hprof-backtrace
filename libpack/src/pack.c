#include "pack_internal.h"

#include "macros.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t vpack(PackStream *stream, const char *tmpl, va_list args)
{
  struct packtemplate_list *list = parse_template(tmpl);
  if(!list)
    return -1;

  size_t start_pos = stream->writepos;
#define WRITE(b,n) \
  if((*stream->vtable->write)(stream, (b), (n)) == 0) \
    goto stop_now;

  for(struct packtemplate_list *l = list; l; l = l->next) {
    for(int c = 0; c < l->item.count; c++) {
      int arrsize;
      int arrsizevalid = 1;
      void *array;
      void *realarray;
      enum packtemplate_type type = l->item.type;
      int bitdepth = l->item.bitdepth;

      switch(l->item.array) {
      case PACK_ARRAY_NONE:
        array = NULL;
        arrsize = 1;
        break;
      case PACK_ARRAY_FIXED:
        array = va_arg(args, void*);
        arrsize = l->item.arrsize;
        break;
      case PACK_ARRAY_DYNAMIC_ARGS:
        array = va_arg(args, void*);
        arrsize = va_arg(args, int);
        break;
      case PACK_ARRAY_DYNAMIC_STREAM:
        // In this case; get the for loop below to write the size for us
        array = NULL;
        realarray = va_arg(args, void*);
        arrsize = 1;
        arrsizevalid = 0;
        type = l->item.arrsize_type;
        bitdepth = l->item.arrsize_bitdepth;
        break;
      }

      int index;
      for(index = 0; index < arrsize; index++) {
        switch(type) {
        case PACK_CHAR:
          {
            char c = array ? ((char*)array)[index] : va_arg(args, int);
            WRITE(&c, sizeof(c));
            if(!arrsizevalid)
              arrsize = c;
          }
          break;
        case PACK_SHORT:
          {
            short s = array ? ((short*)array)[index] : va_arg(args, int);
            WRITE(&s, sizeof(s));
            if(!arrsizevalid)
              arrsize = s;
          }
          break;
        case PACK_INT:
          {
            int i = array ? ((int*)array)[index] : va_arg(args, int);
            WRITE(&i, sizeof(i));
            if(!arrsizevalid)
              arrsize = i;
          }
          break;
        case PACK_LONG:
          {
            long l = array ? ((long*)array)[index] : va_arg(args, long);
            WRITE(&l, sizeof(l));
            if(!arrsizevalid)
              arrsize = l;
          }
          break;
        case PACK_UNSIGNED:
          switch(bitdepth) {
          case 8:
            {
              uint8_t b = array ? ((uint8_t*)array)[index] : va_arg(args, int);
              WRITE(&b, sizeof(b));
              if(!arrsizevalid)
                arrsize = b;
            }
            break;
          case 16:
            {
              uint16_t w = array ? ((uint16_t*)array)[index] : va_arg(args, int);
              if(l->item.endian == PACK_ENDIAN_HOST) {
                WRITE(&w, sizeof(w));
              }
              else {
                unsigned char b[2];
                U16_TO_BYTEARR(b, w, l->item.endian);
                WRITE(b, 2);
              }
              if(!arrsizevalid)
                arrsize = w;
            }
            break;
          case 32:
            {
              uint32_t d = array ? ((uint32_t*)array)[index] : va_arg(args, long);
              if(l->item.endian == PACK_ENDIAN_HOST) {
                WRITE(&d, sizeof(d));
              }
              else {
                unsigned char b[4];
                U32_TO_BYTEARR(b, d, l->item.endian);
                WRITE(b, 4);
              }
              if(!arrsizevalid)
                arrsize = d;
            }
            break;
          case 64:
            {
              uint64_t q = array ? ((uint64_t*)array)[index] : va_arg(args, uint64_t);
              if(l->item.endian == PACK_ENDIAN_HOST) {
                WRITE(&q, sizeof(q));
              }
              else {
                unsigned char b[8];
                U64_TO_BYTEARR(b, q, l->item.endian);
                WRITE(b, 8);
              }
              if(!arrsizevalid)
                arrsize = q;
            }
            break;
          }
          break;
        default:
          ASSERT_UNREACHABLE;
        }

        if(!arrsizevalid) {
          // We've just written the size to the stream. Reset variables for the
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

  return stream->writepos - start_pos;
}

size_t pack(PackStream *stream, const char *tmpl, ...)
{
  va_list args;
  va_start(args, tmpl);

  size_t ret = vpack(stream, tmpl, args);

  va_end(args);
  return ret;
}
