#include <stdio.h>

#include "libpack.h"

#include "test.h"

#include <string.h>

int main(void) {
  char buffer[20];
  size_t len;

  title("snpack()");

  test("u8[5]");
  len = snpack(buffer, sizeof(buffer), "u8[5]", "Hello" );
  size_ok(len, 5);
  int_ok(buffer[0], 'H'); int_ok(buffer[1], 'e'); int_ok(buffer[2], 'l'); int_ok(buffer[3], 'l'); int_ok(buffer[4], 'o');

  test("u8[] with 3");
  len = snpack(buffer, sizeof(buffer), "u8[]", "Hi", 3);
  size_ok(len, 3);
  int_ok(buffer[0], 'H'); int_ok(buffer[1], 'i'); int_ok(buffer[2], 0);

  test("u8[u8] with 0");
  len = snpack(buffer, sizeof(buffer), "u8[u8]", "", 0);
  size_ok(len, 1);
  int_ok(buffer[0], 0);

  test("u8[u8] with 4");
  len = snpack(buffer, sizeof(buffer), "u8[u8]", "1234", 4);
  size_ok(len, 5);
  int_ok(buffer[0], 4);
  int_ok(buffer[1], '1'); int_ok(buffer[2], '2'); int_ok(buffer[3], '3'); int_ok(buffer[4], '4');

  test("u8[u32<] with 0");
  len = snpack(buffer, sizeof(buffer), "u8[u32<]", "", 0);
  size_ok(len, 4);
  int_ok(buffer[0], 0); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);

  test("u8[u32<] with 4");
  len = snpack(buffer, sizeof(buffer), "u8[u32<]", "1234", 4);
  size_ok(len, 8);
  int_ok(buffer[0], 4); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);
  int_ok(buffer[4], '1'); int_ok(buffer[5], '2'); int_ok(buffer[6], '3'); int_ok(buffer[7], '4');

  test("u8[5] count");
  len = snpack(NULL, 0, "u8[5]", "Hello" );
  size_ok(len, 5);

  test("u8[] with 3 count");
  len = snpack(NULL, 0, "u8[]", "Hi", 3);
  size_ok(len, 3);
}
