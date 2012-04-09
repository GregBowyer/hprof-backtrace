#ifndef __LIBPACK_H__
#define __LIBPACK_H__

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

/* generic PackStream functions */

typedef struct _PackStream PackStream;

size_t  pack(PackStream *stream, const char *tmpl, ...);
size_t vpack(PackStream *stream, const char *tmpl, va_list args);

size_t  unpack(PackStream *stream, const char *tmpl, ...);
size_t vunpack(PackStream *stream, const char *tmpl, va_list args);

size_t packstream_remaining(PackStream *stream);

/* string */

PackStream *packstream_string_new(char *buffer, size_t buflen);
PackStream *packstream_conststring_new(const char *buffer, size_t buflen);
void        packstream_string_free(PackStream *stream);

size_t  snpack(char *buffer, size_t buflen, const char *tmpl, ...);
size_t vsnpack(char *buffer, size_t buflen, const char *tmpl, va_list args);

size_t  aspack(char **bufferp, size_t maxlen, const char *tmpl, ...);
size_t vaspack(char **bufferp, size_t maxlen, const char *tmpl, va_list args);

size_t  snunpack(const char *buffer, size_t buflen, const char *tmpl, ...);
size_t vsnunpack(const char *buffer, size_t buflen, const char *tmpl, va_list args);

/* FILE */

PackStream *packstream_file_new(FILE *file);
void        packstream_file_free(PackStream *stream);

size_t  fpack(FILE *file, const char *tmpl, ...);
size_t vfpack(FILE *file, const char *tmpl, va_list args);

size_t  funpack(FILE *file, const char *tmpl, ...);
size_t vfunpack(FILE *file, const char *tmpl, va_list args);

#endif
