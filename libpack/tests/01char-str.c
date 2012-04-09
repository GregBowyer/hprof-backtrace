#include <stdio.h>

#include "libpack.h"

#include "test.h"

#include <stdlib.h>
#include <string.h>

int main(void) {
  char buffer[20];
  size_t len;

  title("snpack()");

  test("c");
  len = snpack(buffer, sizeof(buffer), "c", 0);
  size_ok(len, 1);
  int_ok(buffer[0], 0);

  test("cc");
  len = snpack(buffer, sizeof(buffer), "cc", 1, 2);
  size_ok(len, 2);
  int_ok(buffer[0], 1);
  int_ok(buffer[1], 2);

  test("c c c");
  len = snpack(buffer, sizeof(buffer), "c c c", 3, 4, 5);
  size_ok(len, 3);
  int_ok(buffer[0], 3);
  int_ok(buffer[1], 4);
  int_ok(buffer[2], 5);

  test("c4");
  len = snpack(buffer, sizeof(buffer), "c4", 6, 7, 8, 9);
  size_ok(len, 4);
  int_ok(buffer[0], 6);
  int_ok(buffer[1], 7);
  int_ok(buffer[2], 8);
  int_ok(buffer[3], 9);

  test("c15");
  len = snpack(buffer, sizeof(buffer), "c15", 
      'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n', 0);
  size_ok(len, 15);
  str_ok(buffer, "Hello, world!\n");

  test("cc limit");
  buffer[1] = 0; // reinitialise the buffer to 0
  len = snpack(buffer, 1, "cc", 100, 101);
  size_ok(len, 1);
  int_ok(buffer[0], 100);
  int_ok(buffer[1], 0);

  test("c count");
  len = snpack(NULL, 0, "c", 0);
  size_ok(len, 1);

  test("cc count");
  len = snpack(NULL, 0, "cc", 1, 2);

  title("aspack()");

  char *dynbuffer;

  test("c");
  len = aspack(&dynbuffer, 20, "c", 0);
  size_ok(len, 1);
  int_ok(dynbuffer[0], 0);
  free(dynbuffer);

  test("cc");
  len = aspack(&dynbuffer, 20, "cc", 1, 2);
  size_ok(len, 2);
  int_ok(dynbuffer[0], 1);
  int_ok(dynbuffer[1], 2);
  free(dynbuffer);

  test("c c c");
  len = aspack(&dynbuffer, 20, "c c c", 3, 4, 5);
  size_ok(len, 3);
  int_ok(dynbuffer[0], 3);
  int_ok(dynbuffer[1], 4);
  int_ok(dynbuffer[2], 5);
  free(dynbuffer);

  test("c4");
  len = aspack(&dynbuffer, 20, "c4", 6, 7, 8, 9);
  size_ok(len, 4);
  int_ok(dynbuffer[0], 6);
  int_ok(dynbuffer[1], 7);
  int_ok(dynbuffer[2], 8);
  int_ok(dynbuffer[3], 9);
  free(dynbuffer);

  test("c15");
  len = aspack(&dynbuffer, 20, "c15", 
      'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n', 0);
  size_ok(len, 15);
  str_ok(dynbuffer, "Hello, world!\n");
  free(dynbuffer);

  test("cc limit");
  dynbuffer[1] = 0; // reinitialise the dynbuffer to 0
  len = aspack(&dynbuffer, 1, "cc", 100, 101);
  size_ok(len, 1);
  int_ok(dynbuffer[0], 100);
  int_ok(dynbuffer[1], 0);
  free(dynbuffer);

  title("snunpack()");

  char c1, c2, c3, c4, c5;
  char c6, c7, c8, c9, c10;
  char c11, c12, c13, c14, c15;

  test("c");
  buffer[0] = 0;
  c1 = -1;
  len = snunpack(buffer, sizeof(buffer), "c", &c1);
  size_ok(len, 1);
  int_ok(c1, 0);

  test("cc");
  buffer[0] = 1; buffer[1] = 2;
  c1 = -1; c2 = -1;
  len = snunpack(buffer, sizeof(buffer), "cc", &c1, &c2);
  size_ok(len, 2);
  int_ok(c1, 1);
  int_ok(c2, 2);

  test("c c c");
  buffer[0] = 3; buffer[1] = 4; buffer[2] = 5;
  c1 = -1; c2 = -1; c3 = -1;
  len = snunpack(buffer, sizeof(buffer), "c c c", &c1, &c2, &c3);
  size_ok(len, 3);
  int_ok(c1, 3);
  int_ok(c2, 4);
  int_ok(c3, 5);

  test("c4");
  buffer[0] = 6; buffer[1] = 7; buffer[2] = 8; buffer[3] = 9;
  c1 = -1; c2 = -1; c3 = -1; c4 = -1;
  len = snunpack(buffer, sizeof(buffer), "c4", &c1, &c2, &c3, &c4);
  size_ok(len, 4);
  int_ok(c1, 6);
  int_ok(c2, 7);
  int_ok(c3, 8);
  int_ok(c4, 9);

  test("c15");
  strcpy(buffer, "Hello, world!\n");
  c1 = -1; c2 = -1; c3 = -1; c4 = -1; c5 = -1;
  c6 = -1; c7 = -1; c8 = -1; c9 = -1; c10 = -1;
  c11 = -1; c12 = -1; c13 = -1; c14 = -1; c15 = -1;
  len = snunpack(buffer, sizeof(buffer), "c15", &c1, &c2, &c3, &c4, &c5,
      &c6, &c7, &c8, &c9, &c10, &c11, &c12, &c13, &c14, &c15);
  size_ok(len, 15);
  int_ok(c1, 'H'); int_ok(c2, 'e'); int_ok(c3, 'l'); int_ok(c4, 'l'); int_ok(c5, 'o');
  int_ok(c6, ','); int_ok(c7, ' '); int_ok(c8, 'w'); int_ok(c9, 'o'); int_ok(c10, 'r');
  int_ok(c11, 'l'); int_ok(c12, 'd'); int_ok(c13, '!'); int_ok(c14, '\n'); int_ok(c15, 0);

  return 0;
}
