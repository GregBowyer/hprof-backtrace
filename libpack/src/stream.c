#include "pack_internal.h"

size_t packstream_remaining(PackStream *stream)
{
  return (*stream->vtable->remaining)(stream);
}
