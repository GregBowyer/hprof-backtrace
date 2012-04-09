#include "pack_internal.h"

#include "macros.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// DEBUG
#include <stdio.h>
// END DEBUG

static struct packtemplate_item *append_item(struct packtemplate_list **list)
{
  struct packtemplate_list *n = malloc(sizeof(struct packtemplate_list));
  // TODO - deal with alloc errors
  memset(n, 0, sizeof(struct packtemplate_list));

  n->next = NULL;

  /* Initialse the struct */
  n->item.count = 1;

  if(*list) {
    struct packtemplate_list *tail = *list;
    while(tail->next)
      tail = tail->next;

    tail->next = n;
  }
  else {
    *list = n;
  }

  return &n->item;
}

static const char *skip_ws(const char *s)
{
  while(*s) {
    switch(*s) {
      case ' ': case '\t': case '\n':
        break;
      default:
        return s;
    }
    s++;
  }

  return s;
}

static char parse_int(const char **sp, int *number)
{
  char *endptr;
  *number = strtol(*sp, &endptr, 10);
  // This can't fail - we know we had at least one digit
  *sp = endptr;
  return 1;
}

static char parse_endian(const char **sp, enum packtemplate_endian *endian, int required)
{
  const char *s = skip_ws(*sp);

  switch(*s) {
    case '<': case '>':
      *endian = (**sp == '<') ? PACK_ENDIAN_LITTLE : PACK_ENDIAN_BIG;
      *sp = s+1;
      return 1;

    case 0:
      if(!required)
        return 1;

      fprintf(stderr, "End of template while looking for endian declaration\n");
      return 0;
    default:
      if(!required)
        return 1;

      fprintf(stderr, "Expected endian declaration, found '%c'\n", *s);
      return 0;
  }
}

static char parse_type(const char **sp, enum packtemplate_type *type, enum packtemplate_endian *endian, int *bitdepth)
{
  const char *s = skip_ws(*sp);

  char typechar = *s;

  int need_endian = 1;

  switch(typechar) {
    case 'c': *type = PACK_CHAR;  need_endian = 0; s++; break;
    case 's': *type = PACK_SHORT; need_endian = 0; s++; break;
    case 'i': *type = PACK_INT;   need_endian = 0; s++; break;
    case 'l': *type = PACK_LONG;  need_endian = 0; s++; break;

    case 'u':
      *type = PACK_UNSIGNED;

      s++;
      if(isdigit(*s))
        parse_int(&s, bitdepth);
      else {
        fprintf(stderr, "Expected bitdepth following '%c' format\n", typechar);
        abort();
      }

      switch(*bitdepth) {
      case 8:
        // u8 needs no endian declaration
        need_endian = 0;
        // FALLTHROUGH
      case 16: case 32: case 64:
        break;
      default:
        fprintf(stderr, "Cannot handle non-standard bitdepth of %d\n", *bitdepth);
        abort();
      }
      break;

    // Single byte needs no endian declaration
    case 'b': *type = PACK_UNSIGNED; *bitdepth =  8; need_endian = 0; s++; break;

    case 'w': *type = PACK_UNSIGNED; *bitdepth = 16; s++; break;
    case 'd': *type = PACK_UNSIGNED; *bitdepth = 32; s++; break;
    case 'q': *type = PACK_UNSIGNED; *bitdepth = 64; s++; break;

    default:
      fprintf(stderr, "Unrecognised type character '%c'\n", typechar);
      return 0;
  }

  if(!parse_endian(&s, endian, need_endian))
    return 0;

  *sp = s;
  return 1;
}

struct packtemplate_list *parse_template(const char *tmpl)
{
  const char *t = tmpl;

  struct packtemplate_list *ret = NULL;

  while(*t) {
    t = skip_ws(t);
    if(!*t)
      break;

    char here = *t;

    struct packtemplate_item *cur = append_item(&ret);
    if(!parse_type(&t, &cur->type, &cur->endian, &cur->bitdepth))
      abort();

    t = skip_ws(t);

    if(*t == '[') {
      // Some sort of an array - what kind?
      t++;
      here = *t;

      if(isdigit(here)) {
        cur->array = PACK_ARRAY_FIXED;
        parse_int(&t, &cur->arrsize);
        if(*t != ']') {
          fprintf(stderr, "Bad character following array size '%c'\n", *t);
          abort();
        }
      }
      else if(here == ']')
        cur->array = PACK_ARRAY_DYNAMIC_ARGS;
      else if(strchr("csilubwdq", here)) {
        enum packtemplate_endian endian;
        cur->array = PACK_ARRAY_DYNAMIC_STREAM;
        parse_type(&t, &cur->arrsize_type, &endian, &cur->arrsize_bitdepth);
        if(*t != ']') {
          fprintf(stderr, "Bad character following array size '%c'\n", *t);
          abort();
        }
      }
      else {
        fprintf(stderr, "Bad array type character '%c'\n", here);
        abort();
      }

      t = skip_ws(t+1);
    }

    if(isdigit(*t)) {
      parse_int(&t, &cur->count);

      t = skip_ws(t);
    }
  }

  return ret;
}

void free_template(struct packtemplate_list *list)
{
  while(list) {
    struct packtemplate_list *next = list->next;
    free(list);

    list = next;
  }
}

