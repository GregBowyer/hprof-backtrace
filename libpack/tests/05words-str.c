#include <stdint.h>
#include <stdio.h>

#include "libpack.h"

#include "test.h"

#include <string.h>

int main(void) {
  char buffer[40];
  size_t len;

  title("snpack()");

  test("u8");
  len = snpack(buffer, sizeof(buffer), "u8", 0);
  size_ok(len, 1);
  int_ok(buffer[0], 0);

  test("u8u8");
  len = snpack(buffer, sizeof(buffer), "u8u8", 1, 2);
  size_ok(len, 2);
  int_ok(buffer[0], 1);
  int_ok(buffer[1], 2);

  test("u8u8 limit");
  buffer[1] = 0; // reinitialise the buffer to 0
  len = snpack(buffer, 1, "u8u8", 100, 101);
  size_ok(len, 1);
  int_ok(buffer[0], 100);
  int_ok(buffer[1], 0);

  test("b");
  len = snpack(buffer, sizeof(buffer), "b", 0);
  size_ok(len, 1);
  int_ok(buffer[0], 0);

  test("u16<");
  len = snpack(buffer, sizeof(buffer), "u16<", 10);
  size_ok(len, 2);
  int_ok(buffer[0], 10); int_ok(buffer[1], 0);

  test("u16<u16<");
  len = snpack(buffer, sizeof(buffer), "u16<u16<", 1, 2);
  size_ok(len, 4);
  int_ok(buffer[0], 1); int_ok(buffer[1], 0);
  int_ok(buffer[2], 2); int_ok(buffer[3], 0);

  test("u16>");
  len = snpack(buffer, sizeof(buffer), "u16>", 10);
  size_ok(len, 2);
  int_ok(buffer[0], 0); int_ok(buffer[1], 10);

  test("u16>u16>");
  len = snpack(buffer, sizeof(buffer), "u16>u16>", 1, 2);
  size_ok(len, 4);
  int_ok(buffer[0], 0); int_ok(buffer[1], 1);
  int_ok(buffer[2], 0); int_ok(buffer[3], 2);

  test("u16u16 limit 1");
  len = snpack(buffer, 1, "u16<u16<", 100, 101);
  size_ok(len, 0);

  test("u16u16 limit 2");
  buffer[2] = 0; buffer[3] = 0;
  len = snpack(buffer, 2, "u16<u16<", 100, 101);
  size_ok(len, 2);
  int_ok(buffer[0], 100); int_ok(buffer[1], 0);
  int_ok(buffer[2], 0); int_ok(buffer[3], 0);

  test("w<");
  len = snpack(buffer, sizeof(buffer), "w<", 10);
  size_ok(len, 2);
  int_ok(buffer[0], 10); int_ok(buffer[1], 0);

  // u32 wants a uint32_t. The only portable way to pass that is to cast it long

  test("u32<");
  len = snpack(buffer, sizeof(buffer), "u32<", (long)10);
  size_ok(len, 4);
  int_ok(buffer[0], 10); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);

  test("u32<u32<");
  len = snpack(buffer, sizeof(buffer), "u32<u32<", (long)1, (long)2);
  size_ok(len, 8);
  int_ok(buffer[0], 1); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);
  int_ok(buffer[4], 2); int_ok(buffer[5], 0); int_ok(buffer[6], 0); int_ok(buffer[7], 0);

  test("u32>");
  len = snpack(buffer, sizeof(buffer), "u32>", (long)10);
  size_ok(len, 4);
  int_ok(buffer[0], 0); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 10);

  test("u32>u32>");
  len = snpack(buffer, sizeof(buffer), "u32>u32>", (long)1, (long)2);
  size_ok(len, 8);
  int_ok(buffer[0], 0); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 1);
  int_ok(buffer[4], 0); int_ok(buffer[5], 0); int_ok(buffer[6], 0); int_ok(buffer[7], 2);

  test("u32u32 limit 1");
  len = snpack(buffer, 1, "u32<u32<", (long)100, (long)101);
  size_ok(len, 0);

  test("u32u32 limit 2");
  buffer[4] = 0; buffer[5] = 0; buffer[6] = 0; buffer[7] = 0;
  len = snpack(buffer, 4, "u32<u32<", (long)100, (long)101);
  size_ok(len, 4);
  int_ok(buffer[0], 100); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);
  int_ok(buffer[4], 0); int_ok(buffer[5], 0); int_ok(buffer[6], 0); int_ok(buffer[7], 0);

  test("d<");
  len = snpack(buffer, sizeof(buffer), "d<", (long)10);
  size_ok(len, 4);
  int_ok(buffer[0], 10); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);

  test("u64<");
  len = snpack(buffer, sizeof(buffer), "u64<", (uint64_t)10);
  size_ok(len, 8);
  int_ok(buffer[0], 10); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);
  int_ok(buffer[4],  0); int_ok(buffer[5], 0); int_ok(buffer[6], 0); int_ok(buffer[7], 0);

  test("u64<u64<");
  len = snpack(buffer, sizeof(buffer), "u64<u64<", (uint64_t)10, (uint64_t)11);
  size_ok(len, 16);
  int_ok(buffer[ 0], 10); int_ok(buffer[ 1], 0); int_ok(buffer[ 2], 0); int_ok(buffer[ 3], 0);
  int_ok(buffer[ 4],  0); int_ok(buffer[ 5], 0); int_ok(buffer[ 6], 0); int_ok(buffer[ 7], 0);
  int_ok(buffer[ 8], 11); int_ok(buffer[ 9], 0); int_ok(buffer[10], 0); int_ok(buffer[11], 0);
  int_ok(buffer[12],  0); int_ok(buffer[13], 0); int_ok(buffer[14], 0); int_ok(buffer[15], 0);

  test("u64>");
  len = snpack(buffer, sizeof(buffer), "u64>", (uint64_t)10);
  size_ok(len, 8);
  int_ok(buffer[0], 0); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3],  0);
  int_ok(buffer[4], 0); int_ok(buffer[5], 0); int_ok(buffer[6], 0); int_ok(buffer[7], 10);

  test("u64>u64>");
  len = snpack(buffer, sizeof(buffer), "u64>u64>", (uint64_t)10, (uint64_t)11);
  size_ok(len, 16);
  int_ok(buffer[ 0], 0); int_ok(buffer[ 1], 0); int_ok(buffer[ 2], 0); int_ok(buffer[ 3],  0);
  int_ok(buffer[ 4], 0); int_ok(buffer[ 5], 0); int_ok(buffer[ 6], 0); int_ok(buffer[ 7], 10);
  int_ok(buffer[ 8], 0); int_ok(buffer[ 9], 0); int_ok(buffer[10], 0); int_ok(buffer[11],  0);
  int_ok(buffer[12], 0); int_ok(buffer[13], 0); int_ok(buffer[14], 0); int_ok(buffer[15], 11);

  test("q<");
  len = snpack(buffer, sizeof(buffer), "q<", (uint64_t)10);
  size_ok(len, 8);
  int_ok(buffer[0], 10); int_ok(buffer[1], 0); int_ok(buffer[2], 0); int_ok(buffer[3], 0);
  int_ok(buffer[4],  0); int_ok(buffer[5], 0); int_ok(buffer[6], 0); int_ok(buffer[7], 0);

  title("snunpack()");

  uint8_t b1, b2;

  test("u8");
  buffer[0] = 10;
  b1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u8", &b1);
  size_ok(len, 1);
  int_ok(b1, 10);

  test("u8u8");
  buffer[0] = 1; buffer[1] = 2;
  b1 = 0; b2 = 0;
  len = snunpack(buffer, sizeof(buffer), "u8u8", &b1, &b2);
  size_ok(len, 2);
  int_ok(b1, 1);
  int_ok(b2, 2);

  test("u8u8 limit");
  buffer[0] = 1; buffer[1] = 2;
  b1 = 0; b2 = 0;
  len = snunpack(buffer, 1, "u8u8", &b1, &b2);
  size_ok(len, 1);
  int_ok(b1, 1);
  int_ok(b2, 0);

  test("b");
  buffer[0] = 10;
  b1 = 0;
  len = snunpack(buffer, sizeof(buffer), "b", &b1);
  size_ok(len, 1);
  int_ok(b1, 10);

  uint16_t w1, w2;

  test("u16<");
  buffer[0] = 10; buffer[1] = 0;
  w1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u16<", &w1);
  size_ok(len, 2);
  int_ok(w1, 10);

  test("u16<u16<");
  buffer[0] = 1; buffer[1] = 0;
  buffer[2] = 2; buffer[3] = 0;
  w1 = 0; w2 = 0;
  len = snunpack(buffer, sizeof(buffer), "u16<u16<", &w1, &w2);
  size_ok(len, 4);
  int_ok(w1, 1);
  int_ok(w2, 2);

  test("u16>");
  buffer[0] = 0; buffer[1] = 10;
  w1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u16>", &w1);
  size_ok(len, 2);
  int_ok(w1, 10);

  test("u16>u16>");
  buffer[0] = 0; buffer[1] = 1;
  buffer[2] = 0; buffer[3] = 2;
  w1 = 0; w2 = 0;
  len = snunpack(buffer, sizeof(buffer), "u16>u16>", &w1, &w2);
  size_ok(len, 4);
  int_ok(w1, 1);
  int_ok(w2, 2);

  test("u16u16 limit 1");
  buffer[0] = 1; buffer[1] = 0;
  buffer[2] = 2; buffer[3] = 0;
  w1 = 0; w2 = 0;
  len = snunpack(buffer, 1, "u16<w<", &w1, &w2);
  size_ok(len, 0);

  test("u16u16 limit 2");
  buffer[0] = 1; buffer[1] = 0;
  buffer[2] = 2; buffer[3] = 0;
  w1 = 0; w2 = 0;
  len = snunpack(buffer, 2, "u16<u16<", &w1, &w2);
  size_ok(len, 2);
  int_ok(w1, 1);
  int_ok(w2, 0);

  test("w<");
  buffer[0] = 10; buffer[1] = 0;
  w1 = 0;
  len = snunpack(buffer, sizeof(buffer), "w<", &w1);
  size_ok(len, 2);
  int_ok(w1, 10);

  uint32_t d1, d2;

  test("u32<");
  buffer[0] = 10; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
  d1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u32<", &d1);
  size_ok(len, 4);
  long_ok(d1, 10);

  test("u32< with 128");
  len = snpack(buffer, sizeof(buffer), "u32<", 128);
  size_ok(len, 4);
  d1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u32<", &d1);
  size_ok(len, 4);
  long_ok(d1, 128);

  test("u32<u32<");
  buffer[0] = 1; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
  buffer[4] = 2; buffer[5] = 0; buffer[6] = 0; buffer[7] = 0;
  d1 = 0; d2 = 0;
  len = snunpack(buffer, sizeof(buffer), "u32<u32<", &d1, &d2);
  size_ok(len, 8);
  long_ok(d1, 1);
  long_ok(d2, 2);

  test("u32>");
  buffer[0] = 0; buffer[1] = 0; buffer[2] = 0; buffer[3] = 10;
  d1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u32>", &d1);
  size_ok(len, 4);
  long_ok(d1, 10);

  test("u32>u32>");
  buffer[0] = 0; buffer[1] = 0; buffer[2] = 0; buffer[3] = 1;
  buffer[4] = 0; buffer[5] = 0; buffer[6] = 0; buffer[7] = 2;
  d1 = 0; d2 = 0;
  len = snunpack(buffer, sizeof(buffer), "u32>u32>", &d1, &d2);
  size_ok(len, 8);
  long_ok(d1, 1);
  long_ok(d2, 2);

  test("u32u32 limit 1");
  buffer[0] = 1; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
  buffer[4] = 2; buffer[5] = 0; buffer[6] = 0; buffer[7] = 0;
  d1 = 0; d2 = 0;
  len = snunpack(buffer, 1, "u32<u32<", &d1, &d2);
  size_ok(len, 0);

  test("u32u32 limit 2");
  buffer[0] = 1; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
  buffer[4] = 2; buffer[5] = 0; buffer[6] = 0; buffer[7] = 0;
  d1 = 0; d2 = 0;
  len = snunpack(buffer, 4, "u32<u32<", &d1, &d2);
  size_ok(len, 4);
  long_ok(d1, 1);
  long_ok(d2, 0);

  test("d<");
  buffer[0] = 10; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
  d1 = 0;
  len = snunpack(buffer, sizeof(buffer), "d<", &d1);
  size_ok(len, 4);
  long_ok(d1, 10);

  uint64_t q1, q2;

  test("u64<");
  buffer[0] = 1; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
      buffer[4] = 0; buffer[5] = 0; buffer[6] = 0; buffer[7] = 0;
  q1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u64<", &q1);
  size_ok(len, 8);
  u64_ok(q1, 1);

  test("u64< with 128");
  len = snpack(buffer, sizeof(buffer), "u64<", 128);
  size_ok(len, 8);
  q1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u64<", &q1);
  size_ok(len, 8);
  long_ok(q1, 128);

  test("u64<u64<");
  buffer[0] = 1; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
      buffer[4] = 0; buffer[5] = 0; buffer[6] = 0; buffer[7] = 0;
  buffer[8] = 2; buffer[9] = 0; buffer[10] = 0; buffer[11] = 0;
      buffer[12] = 0; buffer[13] = 0; buffer[14] = 0; buffer[15] = 0;
  q1 = 0; q2 = 0;
  len = snunpack(buffer, sizeof(buffer), "u64<u64<", &q1, &q2);
  size_ok(len, 16);
  u64_ok(q1, 1);
  u64_ok(q2, 2);

  test("u64>");
  buffer[0] = 0; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
      buffer[4] = 0; buffer[5] = 0; buffer[6] = 0; buffer[7] = 1;
  q1 = 0;
  len = snunpack(buffer, sizeof(buffer), "u64>", &q1);
  size_ok(len, 8);
  u64_ok(q1, 1);

  test("u64>u64>");
  buffer[0] = 0; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
      buffer[4] = 0; buffer[5] = 0; buffer[6] = 0; buffer[7] = 1;
  buffer[8] = 0; buffer[9] = 0; buffer[10] = 0; buffer[11] = 0;
      buffer[12] = 0; buffer[13] = 0; buffer[14] = 0; buffer[15] = 2;
  q1 = 0; q2 = 0;
  len = snunpack(buffer, sizeof(buffer), "u64>u64>", &q1, &q2);
  size_ok(len, 16);
  u64_ok(q1, 1);
  u64_ok(q2, 2);

  test("q<");
  buffer[0] = 1; buffer[1] = 0; buffer[2] = 0; buffer[3] = 0;
      buffer[4] = 0; buffer[5] = 0; buffer[6] = 0; buffer[7] = 0;
  q1 = 0;
  len = snunpack(buffer, sizeof(buffer), "q<", &q1);
  size_ok(len, 8);
  u64_ok(q1, 1);

  return 0;
}
