#include <stdint.h>
#include <stdio.h>

#include "libpack.h"

#include "test.h"

#include <string.h>

int main(void) {
  FILE *file = tmpfile();
  size_t len;

  title("fpack()");

  test("u8");
  rewind(file);
  len = fpack(file, "u8", 0);
  size_ok(len, 1);
  rewind(file);
  int_ok(fgetc(file), 0);

  test("u8u8");
  rewind(file);
  len = fpack(file, "u8u8", 1, 2);
  size_ok(len, 2);
  rewind(file);
  int_ok(fgetc(file), 1);
  int_ok(fgetc(file), 2);

  test("b");
  rewind(file);
  len = fpack(file, "b", 0);
  size_ok(len, 1);
  rewind(file);
  int_ok(fgetc(file), 0);

  test("u16<");
  rewind(file);
  len = fpack(file, "u16<", 10);
  size_ok(len, 2);
  rewind(file);
  int_ok(fgetc(file), 10); int_ok(fgetc(file), 0);

  test("u16<u16<");
  rewind(file);
  len = fpack(file, "u16<u16<", 1, 2);
  size_ok(len, 4);
  rewind(file);
  int_ok(fgetc(file), 1); int_ok(fgetc(file), 0);
  int_ok(fgetc(file), 2); int_ok(fgetc(file), 0);

  test("u16>");
  rewind(file);
  len = fpack(file, "u16>", 10);
  size_ok(len, 2);
  rewind(file);
  int_ok(fgetc(file), 0); int_ok(fgetc(file), 10);

  test("u16>u16>");
  rewind(file);
  len = fpack(file, "u16>u16>", 1, 2);
  size_ok(len, 4);
  rewind(file);
  int_ok(fgetc(file), 0); int_ok(fgetc(file), 1);
  int_ok(fgetc(file), 0); int_ok(fgetc(file), 2);

  test("w<");
  rewind(file);
  len = fpack(file, "w<", 10);
  size_ok(len, 2);
  rewind(file);
  int_ok(fgetc(file), 10); int_ok(fgetc(file), 0);

  // d wants a uint32_t. The only portable way to pass that is to cast it long

  test("u32<");
  rewind(file);
  len = fpack(file, "u32<", (long)10);
  size_ok(len, 4);
  rewind(file);
  int_ok(fgetc(file), 10); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0);

  test("u32<u32<");
  rewind(file);
  len = fpack(file, "u32<u32<", (long)1, (long)2);
  size_ok(len, 8);
  rewind(file);
  int_ok(fgetc(file), 1); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0);
  int_ok(fgetc(file), 2); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0);

  test("u32>");
  rewind(file);
  len = fpack(file, "u32>", (long)10);
  rewind(file);
  size_ok(len, 4);
  int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 10);

  test("u32>u32>");
  rewind(file);
  len = fpack(file, "u32>u32>", (long)1, (long)2);
  size_ok(len, 8);
  rewind(file);
  int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 1);
  int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 2);

  test("d<");
  rewind(file);
  len = fpack(file, "d<", (long)10);
  size_ok(len, 4);
  rewind(file);
  int_ok(fgetc(file), 10); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0); int_ok(fgetc(file), 0);

  title("snunpack()");

  uint8_t b1, b2;

  test("u8");
  rewind(file);
  fputc(10, file);
  b1 = 0;
  rewind(file);
  len = funpack(file, "u8", &b1);
  size_ok(len, 1);
  int_ok(b1, 10);

  test("u8u8");
  rewind(file);
  fputc(1, file); fputc(2, file);
  b1 = 0; b2 = 0;
  rewind(file);
  len = funpack(file, "u8u8", &b1, &b2);
  size_ok(len, 2);
  int_ok(b1, 1);
  int_ok(b2, 2);

  test("b");
  rewind(file);
  fputc(10, file);
  b1 = 0;
  rewind(file);
  len = funpack(file, "b", &b1);
  size_ok(len, 1);
  int_ok(b1, 10);

  uint16_t w1, w2;

  test("u16<");
  rewind(file);
  fputc(10, file); fputc(0, file);
  w1 = 0;
  rewind(file);
  len = funpack(file, "u16<", &w1);
  size_ok(len, 2);
  int_ok(w1, 10);

  test("u16<u16<");
  rewind(file);
  fputc(1, file); fputc(0, file);
  fputc(2, file); fputc(0, file);
  w1 = 0; w2 = 0;
  rewind(file);
  len = funpack(file, "u16<u16<", &w1, &w2);
  size_ok(len, 4);
  int_ok(w1, 1);
  int_ok(w2, 2);

  test("u16>");
  rewind(file);
  fputc(0, file); fputc(10, file);
  w1 = 0;
  rewind(file);
  len = funpack(file, "u16>", &w1);
  size_ok(len, 2);
  int_ok(w1, 10);

  test("u16>u16>");
  rewind(file);
  fputc(0, file); fputc(1, file);
  fputc(0, file); fputc(2, file);
  w1 = 0; w2 = 0;
  rewind(file);
  len = funpack(file, "u16>u16>", &w1, &w2);
  size_ok(len, 4);
  int_ok(w1, 1);
  int_ok(w2, 2);

  test("w<");
  rewind(file);
  fputc(10, file); fputc(0, file);
  w1 = 0;
  rewind(file);
  len = funpack(file, "w<", &w1);
  size_ok(len, 2);
  int_ok(w1, 10);

  uint32_t d1, d2;

  test("u32<");
  rewind(file);
  fputc(10, file); fputc(0, file); fputc(0, file); fputc(0, file);
  d1 = 0;
  rewind(file);
  len = funpack(file, "u32<", &d1);
  size_ok(len, 4);
  long_ok(d1, 10);

  test("u32<u32<");
  rewind(file);
  fputc(1, file); fputc(0, file); fputc(0, file); fputc(0, file);
  fputc(2, file); fputc(0, file); fputc(0, file); fputc(0, file);
  d1 = 0; d2 = 0;
  rewind(file);
  len = funpack(file, "u32<u32<", &d1, &d2);
  size_ok(len, 8);
  long_ok(d1, 1);
  long_ok(d2, 2);

  test("u32>");
  rewind(file);
  fputc(0, file); fputc(0, file); fputc(0, file); fputc(10, file);
  d1 = 0;
  rewind(file);
  len = funpack(file, "u32>", &d1);
  size_ok(len, 4);
  long_ok(d1, 10);

  test("u32>u32>");
  rewind(file);
  fputc(0, file); fputc(0, file); fputc(0, file); fputc(1, file);
  fputc(0, file); fputc(0, file); fputc(0, file); fputc(2, file);
  d1 = 0; d2 = 0;
  rewind(file);
  len = funpack(file, "u32>u32>", &d1, &d2);
  size_ok(len, 8);
  long_ok(d1, 1);
  long_ok(d2, 2);

  test("d<");
  rewind(file);
  fputc(10, file); fputc(0, file); fputc(0, file); fputc(0, file);
  d1 = 0;
  rewind(file);
  len = funpack(file, "d<", &d1);
  size_ok(len, 4);
  long_ok(d1, 10);

  return 0;
}
