#include "pack_internal.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct PackStream_String
{
  PackStream s;
  char  *buffer;
  size_t buflen;
};

static size_t readfn(PackStream *stream, void *buffer, size_t n)
{
  struct PackStream_String *ss = (struct PackStream_String *)stream;

  if(stream->readpos + n > ss->buflen)
    return 0;

  memcpy(buffer, ss->buffer + stream->readpos, n);
  stream->readpos += n;

  return n;
}

static size_t writefn(PackStream *stream, const void *buffer, size_t n)
{
  struct PackStream_String *ss = (struct PackStream_String *)stream;

  if(!ss->buffer) {
    /* No buffer; count-only mode */
    stream->writepos += n;
    return n;
  }

  if(stream->writepos + n > ss->buflen)
    return 0;

  memcpy(ss->buffer + stream->writepos, buffer, n);
  stream->writepos += n;

  return n;
}

static size_t remainingfn(PackStream *stream)
{
  struct PackStream_String *ss = (struct PackStream_String *)stream;

  return ss->buflen - stream->readpos;
}

static PackStreamVtable vtable = {
  .read  = &readfn,
  .write = &writefn,

  .remaining = &remainingfn,
};

static PackStreamVtable vtable_const = {
  .read  = &readfn,
  .write = NULL,

  .remaining = &remainingfn,
};

PackStream *packstream_string_new(char *buffer, size_t buflen)
{
  struct PackStream_String *ss = malloc(sizeof(struct PackStream_String));

  ss->s.vtable = &vtable;
  ss->s.readpos = 0;
  ss->s.writepos = 0;
  ss->buffer = buffer;
  ss->buflen = buflen;

  return (PackStream *)ss;
}

PackStream *packstream_conststring_new(const char *buffer, size_t buflen)
{
  struct PackStream_String *ss = malloc(sizeof(struct PackStream_String));

  ss->s.vtable = &vtable_const;
  ss->s.readpos = 0;
  ss->s.writepos = 0;
  /* Yes this is a bit broken, but the read function doesn't modify buffer
   * and there is no write function */
  ss->buffer = (char *)buffer;
  ss->buflen = buflen;

  return (PackStream *)ss;
}

void packstream_string_free(PackStream *stream)
{
  free(stream);
}

size_t vsnpack(char *buffer, size_t buflen, const char *tmpl, va_list args)
{
  PackStream *ss = packstream_string_new(buffer, buflen);

  size_t ret = vpack(ss, tmpl, args);

  packstream_string_free(ss);

  return ret;
}

size_t snpack(char *buffer, size_t buflen, const char *tmpl, ...)
{
  va_list args;
  va_start(args, tmpl);

  size_t ret = vsnpack(buffer, buflen, tmpl, args);

  va_end(args);
  return ret;
}

size_t vaspack(char **bufferp, size_t maxlen, const char *tmpl, va_list args)
{
  /* TODO: This function is a bit inefficient, as it traverses the template
   * twice. Better would be a new type of stream which allocates buffer chunks
   * as it goes, and returns them all joined up at the end
   */

  PackStream *countstream = packstream_string_new(NULL, 0);

  va_list countargs;
  va_copy(countargs, args);

  size_t len = vpack(countstream, tmpl, countargs);

  packstream_string_free(countstream);
  va_end(countargs);

  if(len < 0)
    return len;

  if(maxlen && len > maxlen)
    len = maxlen;

  *bufferp = malloc(len);
  if(!*bufferp)
    return -1;

  return vsnpack(*bufferp, len, tmpl, args);
}

size_t aspack(char **bufferp, size_t maxlen, const char *tmpl, ...)
{
  va_list args;
  va_start(args, tmpl);

  size_t ret = vaspack(bufferp, maxlen, tmpl, args);

  va_end(args);
  return ret;
}

size_t vsnunpack(const char *buffer, size_t buflen, const char *tmpl, va_list args)
{
  PackStream *ss = packstream_conststring_new(buffer, buflen);

  size_t ret = vunpack(ss, tmpl, args);

  packstream_string_free(ss);

  return ret;
}

size_t snunpack(const char *buffer, size_t buflen, const char *tmpl, ...)
{
  va_list args;
  va_start(args, tmpl);

  size_t ret = vsnunpack(buffer, buflen, tmpl, args);

  va_end(args);
  return ret;
}
