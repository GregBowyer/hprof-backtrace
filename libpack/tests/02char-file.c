#include <stdio.h>

#include "libpack.h"

#include "test.h"

int main(void) {
  FILE *file = tmpfile();
  size_t len;

  title("fpack()");

  test("c");
  rewind(file);
  len = fpack(file, "c", 0);
  size_ok(len, 1);
  rewind(file);
  int_ok(fgetc(file), 0);

  test("cc");
  rewind(file);
  len = fpack(file, "cc", 1, 2);
  size_ok(len, 2);
  rewind(file);
  int_ok(fgetc(file), 1);
  int_ok(fgetc(file), 2);

  test("c c c");
  rewind(file);
  len = fpack(file, "c c c", 3, 4, 5);
  size_ok(len, 3);
  rewind(file);
  int_ok(fgetc(file), 3);
  int_ok(fgetc(file), 4);
  int_ok(fgetc(file), 5);

  test("c4");
  rewind(file);
  len = fpack(file, "c4", 6, 7, 8, 9);
  size_ok(len, 4);
  rewind(file);
  int_ok(fgetc(file), 6);
  int_ok(fgetc(file), 7);
  int_ok(fgetc(file), 8);
  int_ok(fgetc(file), 9);

  test("c15");
  rewind(file);
  len = fpack(file, "c15", 
      'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n', 0);
  size_ok(len, 15);
  rewind(file);
  char buffer[20];
  fread(buffer, 1, len, file);
  str_ok(buffer, "Hello, world!\n");

  title("funpack()");

  char c1, c2, c3, c4, c5;
  char c6, c7, c8, c9, c10;
  char c11, c12, c13, c14, c15;

  test("c");
  rewind(file);
  fputc(0, file);
  c1 = -1;
  rewind(file);
  len = funpack(file, "c", &c1);
  size_ok(len, 1);
  int_ok(c1, 0);

  test("cc");
  rewind(file);
  fputc(1, file); fputc(2, file);
  c1 = -1; c2 = -1;
  rewind(file);
  len = funpack(file, "cc", &c1, &c2);
  size_ok(len, 2);
  int_ok(c1, 1);
  int_ok(c2, 2);

  test("c c c");
  rewind(file);
  fputc(3, file); fputc(4, file); fputc(5, file);
  c1 = -1; c2 = -1; c3 = -1;
  rewind(file);
  len = funpack(file, "c c c", &c1, &c2, &c3);
  size_ok(len, 3);
  int_ok(c1, 3);
  int_ok(c2, 4);
  int_ok(c3, 5);

  test("c4");
  rewind(file);
  fputc(6, file); fputc(7, file); fputc(8, file); fputc(9, file);
  c1 = -1; c2 = -1; c3 = -1; c4 = -1;
  rewind(file);
  len = funpack(file, "c4", &c1, &c2, &c3, &c4);
  size_ok(len, 4);
  int_ok(c1, 6);
  int_ok(c2, 7);
  int_ok(c3, 8);
  int_ok(c4, 9);

  test("c15");
  rewind(file);
  fputs("Hello, world!\n", file);
  c1 = -1; c2 = -1; c3 = -1; c4 = -1; c5 = -1;
  c6 = -1; c7 = -1; c8 = -1; c9 = -1; c10 = -1;
  c11 = -1; c12 = -1; c13 = -1; c14 = -1; c15 = -1;
  rewind(file);
  len = funpack(file, "c15", &c1, &c2, &c3, &c4, &c5,
      &c6, &c7, &c8, &c9, &c10, &c11, &c12, &c13, &c14, &c15);
  size_ok(len, 15);
  int_ok(c1, 'H'); int_ok(c2, 'e'); int_ok(c3, 'l'); int_ok(c4, 'l'); int_ok(c5, 'o');
  int_ok(c6, ','); int_ok(c7, ' '); int_ok(c8, 'w'); int_ok(c9, 'o'); int_ok(c10, 'r');
  int_ok(c11, 'l'); int_ok(c12, 'd'); int_ok(c13, '!'); int_ok(c14, '\n'); int_ok(c15, 0);

  return 0;
}
