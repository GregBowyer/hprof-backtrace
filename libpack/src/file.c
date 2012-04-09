#include "pack_internal.h"

#include <stdint.h>
#include <stdlib.h>

struct PackStream_File
{
  PackStream s;
  FILE *f;
};

static size_t readfn(PackStream *stream, void *buffer, size_t n)
{
  struct PackStream_File *fs = (struct PackStream_File *)stream;

  if(fread(buffer, n, 1, fs->f) < 1)
    return 0;
  stream->readpos += n;
  return n;
}

static size_t writefn(PackStream *stream, const void *buffer, size_t n)
{
  struct PackStream_File *fs = (struct PackStream_File *)stream;

  if(fwrite(buffer, n, 1, fs->f) < 1)
    return 0;
  stream->writepos += n;
  return n;
}

static size_t remainingfn(PackStream *stream)
{
  return (size_t)-1;
}

static PackStreamVtable vtable = {
  .read  = &readfn,
  .write = &writefn,

  .remaining = &remainingfn,
};

PackStream *packstream_file_new(FILE *file)
{
  struct PackStream_File *fs = malloc(sizeof(struct PackStream_File));

  fs->s.vtable = &vtable;
  fs->s.readpos = 0;
  fs->s.writepos = 0;
  fs->f = file;

  return (PackStream *)fs;
}

void packstream_file_free(PackStream *stream)
{
  free(stream);
}

size_t vfpack(FILE *file, const char *tmpl, va_list args)
{
  PackStream *fs = packstream_file_new(file);

  size_t ret = vpack(fs, tmpl, args);

  packstream_file_free(fs);

  return ret;
}

size_t fpack(FILE *file, const char *tmpl, ...)
{
  va_list args;
  va_start(args, tmpl);

  size_t ret = vfpack(file, tmpl, args);

  va_end(args);
  return ret;
}

size_t vfunpack(FILE *file, const char *tmpl, va_list args)
{
  PackStream *fs = packstream_file_new(file);

  size_t ret = vunpack(fs, tmpl, args);

  packstream_file_free(fs);

  return ret;
}

size_t funpack(FILE *file, const char *tmpl, ...)
{
  va_list args;
  va_start(args, tmpl);

  size_t ret = vfunpack(file, tmpl, args);

  va_end(args);
  return ret;
}
