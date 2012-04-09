#include <stdio.h>

#include "libpack.h"

#include "test.h"

int main(void) {
  FILE *file = tmpfile();
  size_t len;

  title("fpack()");

  test("c[5]");
  rewind(file);
  len = fpack(file, "c[5]", "Hello" );
  size_ok(len, 5);
  rewind(file);
  int_ok(fgetc(file), 'H'); int_ok(fgetc(file), 'e'); int_ok(fgetc(file), 'l'); int_ok(fgetc(file), 'l'); int_ok(fgetc(file), 'o');

  test("c[] with 3");
  rewind(file);
  len = fpack(file, "c[]", "Hi", 3);
  size_ok(len, 3);
  rewind(file);

  test("c[c] with 0");
  rewind(file);
  len = fpack(file, "c[c]", "", 0);
  size_ok(len, 1);
  rewind(file);
  int_ok(fgetc(file), 0);

  test("c[c] with 4");
  rewind(file);
  len = fpack(file, "c[c]", "1234", 4);
  size_ok(len, 5);
  rewind(file);
  int_ok(fgetc(file), 4);
  int_ok(fgetc(file), '1'); int_ok(fgetc(file), '2'); int_ok(fgetc(file), '3'); int_ok(fgetc(file), '4');

  title("funpack()");

  char c[5];

  test("c[5]");
  rewind(file);
  fputs("Bye!!", file);
  rewind(file);
  len = funpack(file, "c[5]", c);
  size_ok(len, 5);
  int_ok(c[0], 'B'); int_ok(c[1], 'y'); int_ok(c[2], 'e'); int_ok(c[3], '!'); int_ok(c[4], '!');

  test("c[] with 3");
  rewind(file);
  fputs("gon", file); // gone is 4 chars
  rewind(file);
  len = funpack(file, "c[]", c, 3);
  size_ok(len, 3);
  int_ok(c[0], 'g'); int_ok(c[1], 'o'); int_ok(c[2], 'n');

  char count;

  test("c[c] with 0");
  rewind(file);
  fputc('\0', file);
  count = 5;
  rewind(file);
  len = funpack(file, "c[c]", c, &count);
  size_ok(len, 1);
  int_ok(count, 0);

  test("c[c] with 4");
  rewind(file);
  fputs("\0045678", file);
  count = 5;
  rewind(file);
  len = funpack(file, "c[c]", c, &count);
  size_ok(len, 5);
  int_ok(count, 4);
  int_ok(c[0], '5'); int_ok(c[1], '6'); int_ok(c[2], '7'); int_ok(c[3], '8');

  test("c[c] limit");
  rewind(file);
  fputs("\0069abcde", file);
  c[1] = 0;
  count = 1;
  rewind(file);
  len = funpack(file, "c[c]", c, &count);
  size_ok(len, 7);
  int_ok(count, 1);
  int_ok(c[0], '9');
  int_ok(c[1], 0);

  return 0;
}
