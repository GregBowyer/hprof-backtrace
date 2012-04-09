#include <stdio.h>

#include "libpack.h"

#include "test.h"

// The "s" format packs host-endian host-sized "short"s by default, so this
// test script is specifically endian-and-size dependant for a reason.
// Ditto for "i" and "l"
#include <string.h> // for memcpy()

int main(void) {
  short shorts[10];
  int ints[10];
  long longs[20];
  char buffer[10*sizeof(long)];
  size_t len;

  title("snpack()");

  test("s");
  len = snpack(buffer, sizeof(buffer), "s", 0);
  size_ok(len, 1*sizeof(short));
  memcpy(shorts, buffer, len);
  int_ok(shorts[0], 0);

  test("ss");
  len = snpack(buffer, sizeof(buffer), "ss", 1, 2);
  size_ok(len, 2*sizeof(short));
  memcpy(shorts, buffer, len);
  int_ok(shorts[0], 1);
  int_ok(shorts[1], 2);

  test("ss limit 1");
  buffer[1] = 0; // reinitialise the buffer to 0
  len = snpack(buffer, 1, "ss", 100, 101);
  size_ok(len, 0);

  test("ss limit 2");
  memset(buffer + sizeof(short), 0, sizeof(short)); // reinitialise the buffer to 0
  len = snpack(buffer, sizeof(short), "ss", 100, 101);
  size_ok(len, sizeof(short));
  memcpy(shorts, buffer, 2*sizeof(short));
  int_ok(shorts[0], 100);
  int_ok(shorts[1], 0);

  test("i");
  len = snpack(buffer, sizeof(buffer), "i", 0);
  size_ok(len, 1*sizeof(int));
  memcpy(ints, buffer, len);
  int_ok(ints[0], 0);

  test("ii");
  len = snpack(buffer, sizeof(buffer), "ii", 1, 2);
  size_ok(len, 2*sizeof(int));
  memcpy(ints, buffer, len);
  int_ok(ints[0], 1);
  int_ok(ints[1], 2);

  test("ii limit 1");
  buffer[1] = 0; // reinitialise the buffer to 0
  len = snpack(buffer, 1, "ii", 100, 101);
  size_ok(len, 0);

  test("ii limit 2");
  memset(buffer + sizeof(int), 0, sizeof(int)); // reinitialise the buffer to 0
  len = snpack(buffer, sizeof(int), "ii", 100, 101);
  size_ok(len, sizeof(int));
  memcpy(ints, buffer, 2*sizeof(int));
  int_ok(ints[0], 100);
  int_ok(ints[1], 0);

  test("l");
  len = snpack(buffer, sizeof(buffer), "l", 0L);
  long_ok(len, 1*sizeof(long));
  memcpy(longs, buffer, len);
  long_ok(longs[0], 0);

  test("ll");
  len = snpack(buffer, sizeof(buffer), "ll", 1L, 2L);
  long_ok(len, 2*sizeof(long));
  memcpy(longs, buffer, len);
  long_ok(longs[0], 1);
  long_ok(longs[1], 2);

  test("ll limit 1");
  buffer[1] = 0; // reinitialise the buffer to 0
  len = snpack(buffer, 1, "ll", 100L, 101L);
  long_ok(len, 0);

  test("ll limit 2");
  memset(buffer + sizeof(long), 0, sizeof(long)); // reinitialise the buffer to 0
  len = snpack(buffer, sizeof(long), "ll", 100L, 101L);
  long_ok(len, sizeof(long));
  memcpy(longs, buffer, 2*sizeof(long));
  long_ok(longs[0], 100);
  long_ok(longs[1], 0);

  title("snunpack()");

  short s1, s2;

  test("s");
  shorts[0] = 0;
  s1 = -1;
  memcpy(buffer, shorts, 1*sizeof(short));
  len = snunpack(buffer, sizeof(buffer), "s", &s1);
  size_ok(len, 1*sizeof(short));
  int_ok(s1, 0);

  test("ss");
  shorts[0] = 1;
  shorts[1] = 2;
  s1 = -1;
  s2 = -1;
  memcpy(buffer, shorts, 2*sizeof(short));
  len = snunpack(buffer, sizeof(buffer), "ss", &s1, &s2);
  size_ok(len, 2*sizeof(short));
  int_ok(s1, 1);
  int_ok(s2, 2);

  test("ss limit 1");
  shorts[0] = 0;
  s1 = -1;
  s2 = -1;
  memcpy(buffer, shorts, 1*sizeof(short));
  len = snunpack(buffer, 1, "ss", &s1, &s2);
  size_ok(len, 0);

  test("ss limit 2");
  shorts[0] = 100;
  shorts[1] = 101;
  s1 = -1;
  s2 = -1;
  memcpy(buffer, shorts, 2*sizeof(short));
  len = snunpack(buffer, sizeof(short), "ss", &s1, &s2);
  size_ok(len, sizeof(short));
  int_ok(s1, 100);
  int_ok(s2, -1);

  int i1, i2;

  test("i");
  ints[0] = 0;
  i1 = -1;
  memcpy(buffer, ints, 1*sizeof(int));
  len = snunpack(buffer, sizeof(buffer), "i", &i1);
  size_ok(len, 1*sizeof(int));
  int_ok(i1, 0);

  test("ii");
  ints[0] = 1;
  ints[1] = 2;
  i1 = -1;
  i2 = -1;
  memcpy(buffer, ints, 2*sizeof(int));
  len = snunpack(buffer, sizeof(buffer), "ii", &i1, &i2);
  size_ok(len, 2*sizeof(int));
  int_ok(i1, 1);
  int_ok(i2, 2);

  test("ii limit 1");
  ints[0] = 0;
  i1 = -1;
  i2 = -1;
  memcpy(buffer, ints, 1*sizeof(int));
  len = snunpack(buffer, 1, "ii", &i1, &i2);
  size_ok(len, 0);

  test("ii limit 2");
  ints[0] = 100;
  ints[1] = 101;
  i1 = -1;
  i2 = -1;
  memcpy(buffer, ints, 2*sizeof(int));
  len = snunpack(buffer, sizeof(int), "ii", &i1, &i2);
  size_ok(len, sizeof(int));
  int_ok(i1, 100);
  int_ok(i2, -1);

  long l1, l2;

  test("l");
  longs[0] = 0;
  l1 = -1;
  memcpy(buffer, longs, 1*sizeof(long));
  len = snunpack(buffer, sizeof(buffer), "l", &l1);
  size_ok(len, 1*sizeof(long));
  long_ok(l1, 0);

  test("ll");
  longs[0] = 1;
  longs[1] = 2;
  l1 = -1;
  l2 = -1;
  memcpy(buffer, longs, 2*sizeof(long));
  len = snunpack(buffer, sizeof(buffer), "ll", &l1, &l2);
  size_ok(len, 2*sizeof(long));
  long_ok(l1, 1);
  long_ok(l2, 2);

  test("ll limit 1");
  longs[0] = 0;
  s1 = -1;
  s2 = -1;
  memcpy(buffer, longs, 1*sizeof(long));
  len = snunpack(buffer, 1, "ll", &s1, &s2);
  size_ok(len, 0);

  test("ll limit 2");
  longs[0] = 100;
  longs[1] = 101;
  l1 = -1;
  l2 = -1;
  memcpy(buffer, longs, 2*sizeof(long));
  len = snunpack(buffer, sizeof(long), "ll", &l1, &l2);
  size_ok(len, sizeof(long));
  long_ok(l1, 100);
  long_ok(l2, -1);

  return 0;
}
