#include <stdio.h>

#include "libpack.h"

#include "test.h"

#include <string.h>

int main(void) {
  char buffer[20];
  size_t len;

  title("snpack()");

  test("c[5]");
  len = snpack(buffer, sizeof(buffer), "c[5]", "Hello" );
  size_ok(len, 5);
  int_ok(buffer[0], 'H'); int_ok(buffer[1], 'e'); int_ok(buffer[2], 'l'); int_ok(buffer[3], 'l'); int_ok(buffer[4], 'o');

  test("c[] with 3");
  len = snpack(buffer, sizeof(buffer), "c[]", "Hi", 3);
  size_ok(len, 3);
  int_ok(buffer[0], 'H'); int_ok(buffer[1], 'i'); int_ok(buffer[2], 0);

  test("c[c] with 0");
  len = snpack(buffer, sizeof(buffer), "c[c]", "", 0);
  size_ok(len, 1);
  int_ok(buffer[0], 0);

  test("c[c] with 4");
  len = snpack(buffer, sizeof(buffer), "c[c]", "1234", 4);
  size_ok(len, 5);
  int_ok(buffer[0], 4);
  int_ok(buffer[1], '1'); int_ok(buffer[2], '2'); int_ok(buffer[3], '3'); int_ok(buffer[4], '4');

  test("c[5] count");
  len = snpack(NULL, 0, "c[5]", "Hello" );
  size_ok(len, 5);

  test("c[] with 3 count");
  len = snpack(NULL, 0, "c[]", "Hi", 3);
  size_ok(len, 3);

  title("aspack()");

  char *dynbuffer;

  test("c[5]");
  len = aspack(&dynbuffer, 20, "c[5]", "Hello" );
  size_ok(len, 5);
  int_ok(dynbuffer[0], 'H'); int_ok(dynbuffer[1], 'e'); int_ok(dynbuffer[2], 'l'); int_ok(dynbuffer[3], 'l'); int_ok(dynbuffer[4], 'o');

  test("c[] with 3");
  len = aspack(&dynbuffer, 20, "c[]", "Hi", 3);
  size_ok(len, 3);
  int_ok(dynbuffer[0], 'H'); int_ok(dynbuffer[1], 'i'); int_ok(dynbuffer[2], 0);

  test("c[c] with 0");
  len = aspack(&dynbuffer, 20, "c[c]", "", 0);
  size_ok(len, 1);
  int_ok(dynbuffer[0], 0);

  test("c[c] with 4");
  len = aspack(&dynbuffer, 20, "c[c]", "1234", 4);
  size_ok(len, 5);
  int_ok(dynbuffer[0], 4);
  int_ok(dynbuffer[1], '1'); int_ok(dynbuffer[2], '2'); int_ok(dynbuffer[3], '3'); int_ok(dynbuffer[4], '4');

  title("snunpack()");

  char c[5];

  test("c[5]");
  memcpy(buffer, "Bye!!", 5);
  len = snunpack(buffer, sizeof(buffer), "c[5]", c);
  size_ok(len, 5);
  int_ok(c[0], 'B'); int_ok(c[1], 'y'); int_ok(c[2], 'e'); int_ok(c[3], '!'); int_ok(c[4], '!');

  test("c[] with 3");
  memcpy(buffer, "gon", 3); // gone is 4 chars
  len = snunpack(buffer, sizeof(buffer), "c[]", c, 3);
  size_ok(len, 3);
  int_ok(c[0], 'g'); int_ok(c[1], 'o'); int_ok(c[2], 'n');

  char count;

  test("c[c] with 0");
  memcpy(buffer, "\0", 1);
  count = 5;
  len = snunpack(buffer, sizeof(buffer), "c[c]", c, &count);
  size_ok(len, 1);
  int_ok(count, 0);

  test("c[c] with 4");
  memcpy(buffer, "\0045678", 5);
  count = 5;
  len = snunpack(buffer, sizeof(buffer), "c[c]", c, &count);
  size_ok(len, 5);
  int_ok(count, 4);
  int_ok(c[0], '5'); int_ok(c[1], '6'); int_ok(c[2], '7'); int_ok(c[3], '8');

  test("c[c] limit");
  memcpy(buffer, "\0069abcde", 7);
  c[1] = 0;
  count = 1;
  len = snunpack(buffer, sizeof(buffer), "c[c]", c, &count);
  size_ok(len, 7);
  int_ok(count, 1);
  int_ok(c[0], '9');
  int_ok(c[1], 0);

  return 0;
}
