#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

void title(const char *title)
{
  printf("=======\n  %s\n=======\n", title);
}

void test(const char *message)
{
  printf("** %s **\n", message);
}

void size_ok(size_t got, size_t expected)
{
  if(got == expected)
    return;

  fprintf(stderr, "Got %d bytes, expected %d bytes\n", got, expected);
  exit(1);
}

void int_ok(int got, int expected)
{
  if(got == expected)
    return;

  fprintf(stderr, "Got %d, expected %d\n", got, expected);
  exit(1);
}

void long_ok(long got, long expected)
{
  if(got == expected)
    return;

  fprintf(stderr, "Got %ld, expected %ld\n", got, expected);
  exit(1);
}

void u64_ok(uint64_t got, uint64_t expected)
{
  if(got == expected)
    return;

  fprintf(stderr, "Got %" PRIu64 ", expected %" PRIu64 "\n", got, expected);
  exit(1);
}

void str_ok(const char* got, const char* expected)
{
  if(strcmp(got, expected) == 0)
    return;

  fprintf(stderr, "Got '%s', expected '%s'\n", got, expected);
  exit(1);
}
