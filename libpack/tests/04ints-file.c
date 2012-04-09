#include <stdio.h>

#include "libpack.h"

#include "test.h"

// The "s" format packs host-endian host-sized "short"s by default, so this
// test script is specifically endian-and-size dependant for a reason.
// Ditto for "i" and "l"
#include <string.h> // for memcpy()

short fget_short(FILE *file)
{
  short ret;
  fread(&ret, sizeof(short), 1, file);
  return ret;
}

int fget_int(FILE *file)
{
  int ret;
  fread(&ret, sizeof(int), 1, file);
  return ret;
}

long fget_long(FILE *file)
{
  long ret;
  fread(&ret, sizeof(long), 1, file);
  return ret;
}

void fput_short(short val, FILE *file)
{
  fwrite(&val, sizeof(short), 1, file);
}

void fput_int(int val, FILE *file)
{
  fwrite(&val, sizeof(int), 1, file);
}

void fput_long(long val, FILE *file)
{
  fwrite(&val, sizeof(long), 1, file);
}

int main(void) {
  FILE *file = tmpfile();
  size_t len;

  title("fpack()");

  test("s");
  rewind(file);
  len = fpack(file, "s", 0);
  size_ok(len, 1*sizeof(short));
  rewind(file);
  int_ok(fget_short(file), 0);

  test("ss");
  rewind(file);
  len = fpack(file, "ss", 1, 2);
  size_ok(len, 2*sizeof(short));
  rewind(file);
  int_ok(fget_short(file), 1);
  int_ok(fget_short(file), 2);

  test("i");
  rewind(file);
  len = fpack(file, "i", 0);
  size_ok(len, 1*sizeof(int));
  rewind(file);
  int_ok(fget_int(file), 0);

  test("ii");
  rewind(file);
  len = fpack(file, "ii", 1, 2);
  size_ok(len, 2*sizeof(int));
  rewind(file);
  int_ok(fget_int(file), 1);
  int_ok(fget_int(file), 2);

  test("l");
  rewind(file);
  len = fpack(file, "l", 0L);
  long_ok(len, 1*sizeof(long));
  rewind(file);
  long_ok(fget_long(file), 0);

  test("ll");
  rewind(file);
  len = fpack(file, "ll", 1L, 2L);
  long_ok(len, 2*sizeof(long));
  rewind(file);
  long_ok(fget_long(file), 1);
  long_ok(fget_long(file), 2);

  title("funpack()");

  short s1, s2;

  test("s");
  rewind(file);
  fput_short(0, file);
  rewind(file);
  len = funpack(file, "s", &s1);
  size_ok(len, 1*sizeof(short));
  int_ok(s1, 0);

  test("ss");
  rewind(file);
  fput_short(1, file);
  fput_short(2, file);
  rewind(file);
  len = funpack(file, "ss", &s1, &s2);
  size_ok(len, 2*sizeof(short));
  int_ok(s1, 1);
  int_ok(s2, 2);

  int i1, i2;

  test("i");
  rewind(file);
  fput_int(0, file);
  rewind(file);
  len = funpack(file, "i", &i1);
  size_ok(len, 1*sizeof(int));
  int_ok(i1, 0);

  test("ii");
  rewind(file);
  fput_int(1, file);
  fput_int(2, file);
  rewind(file);
  len = funpack(file, "ii", &i1, &i2);
  size_ok(len, 2*sizeof(int));
  int_ok(i1, 1);
  int_ok(i2, 2);

  long l1, l2;

  test("l");
  rewind(file);
  fput_long(0, file);
  rewind(file);
  len = funpack(file, "l", &l1);
  size_ok(len, 1*sizeof(long));
  long_ok(l1, 0);

  test("ll");
  rewind(file);
  fput_long(1, file);
  fput_long(2, file);
  rewind(file);
  len = funpack(file, "ll", &l1, &l2);
  size_ok(len, 2*sizeof(long));
  long_ok(l1, 1);
  long_ok(l2, 2);

  return 0;
}
