#ifndef __TEST_H__
#define __TEST_H__

#include <inttypes.h>

void title(const char *title);
void test(const char *message);
void size_ok(size_t got, size_t expected);
void int_ok(int got, int expected);
void long_ok(long got, long expected);
void u64_ok(uint64_t got, uint64_t expected);
void str_ok(const char *got, const char *expected);

#endif
