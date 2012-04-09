#ifndef __MACROS_H__
#define __MACROS_H__

// Macros to transfer values to/from byte arrays

#define U16_TO_BYTEARR(b, w, e) \
  switch(e) { \
  case PACK_ENDIAN_LITTLE: \
    (b)[0] = (w) & 0xff; (b)[1] = ((w) >> 8) & 0xff; break; \
  case PACK_ENDIAN_BIG: \
    (b)[1] = (w) & 0xff; (b)[0] = ((w) >> 8) & 0xff; break; \
  case PACK_ENDIAN_HOST: abort(); \
  }

#define BYTEARR_TO_U16(b, w, e) \
  switch(e) { \
  case PACK_ENDIAN_LITTLE: \
    (w) = ((b)[1] << 8) | (b)[0]; break; \
  case PACK_ENDIAN_BIG: \
    (w) = ((b)[0] << 8) | (b)[1]; break; \
  case PACK_ENDIAN_HOST: abort(); \
  }

#define U32_TO_BYTEARR(b, d, e) \
  switch(e) { \
  case PACK_ENDIAN_LITTLE: \
    (b)[0] = ((d)      ) & 0xff; (b)[1] = ((d) >>  8) & 0xff; \
    (b)[2] = ((d) >> 16) & 0xff; (b)[3] = ((d) >> 24) & 0xff; break; \
  case PACK_ENDIAN_BIG: \
    (b)[3] = ((d)      ) & 0xff; (b)[2] = ((d) >>  8) & 0xff; \
    (b)[1] = ((d) >> 16) & 0xff; (b)[0] = ((d) >> 24) & 0xff; break; \
  case PACK_ENDIAN_HOST: abort(); \
  }

#define BYTEARR_TO_U32(b, d, e) \
  switch(e) { \
  case PACK_ENDIAN_LITTLE: \
    (d) = ((uint32_t)(b)[3] << 24) | ((uint32_t)(b)[2] << 16) | \
          ((uint32_t)(b)[1] <<  8) | ((uint32_t)(b)[0]      ); break; \
  case PACK_ENDIAN_BIG: \
    (d) = ((uint32_t)(b)[0] << 24) | ((uint32_t)(b)[1] << 16) | \
          ((uint32_t)(b)[2] <<  8) | ((uint32_t)(b)[3]      ); break; \
  case PACK_ENDIAN_HOST: abort(); \
  }

#define U64_TO_BYTEARR(b, q, e) \
  switch(e) { \
  case PACK_ENDIAN_LITTLE: \
    (b)[0] = ((q)      ) & 0xff; (b)[1] = ((q) >>  8) & 0xff; \
    (b)[2] = ((q) >> 16) & 0xff; (b)[3] = ((q) >> 24) & 0xff; \
    (b)[4] = ((q) >> 32) & 0xff; (b)[5] = ((q) >> 40) & 0xff; \
    (b)[6] = ((q) >> 48) & 0xff; (b)[7] = ((q) >> 56) & 0xff; break; \
  case PACK_ENDIAN_BIG: \
    (b)[7] = ((q)      ) & 0xff; (b)[6] = ((q) >>  8) & 0xff; \
    (b)[5] = ((q) >> 16) & 0xff; (b)[4] = ((q) >> 24) & 0xff; \
    (b)[3] = ((q) >> 32) & 0xff; (b)[2] = ((q) >> 40) & 0xff; \
    (b)[1] = ((q) >> 48) & 0xff; (b)[0] = ((q) >> 56) & 0xff; break; \
  case PACK_ENDIAN_HOST: abort(); \
  }

#define BYTEARR_TO_U64(b, q, e) \
  switch(e) { \
  case PACK_ENDIAN_LITTLE: \
    (q) = ((uint64_t)(b)[7] << 56) | ((uint64_t)(b)[6] << 48) | \
          ((uint64_t)(b)[5] << 40) | ((uint64_t)(b)[4] << 32) | \
          ((uint64_t)(b)[3] << 24) | ((uint64_t)(b)[2] << 16) | \
          ((uint64_t)(b)[1] <<  8) | ((uint64_t)(b)[0]      ); break; \
  case PACK_ENDIAN_BIG: \
    (q) = ((uint64_t)(b)[0] << 56) | ((uint64_t)(b)[1] << 48) | \
          ((uint64_t)(b)[2] << 40) | ((uint64_t)(b)[3] << 32) | \
          ((uint64_t)(b)[4] << 24) | ((uint64_t)(b)[5] << 16) | \
          ((uint64_t)(b)[6] <<  8) | ((uint64_t)(b)[7]      ); break; \
  case PACK_ENDIAN_HOST: abort(); \
  }

// Miscellaneous macros

#define ASSERT_UNREACHABLE \
  do { \
    fprintf(stderr, "Assert failure - unreachable line %s:%d - aborting\n", __FILE__, __LINE__); \
    abort(); \
  } while(0)

#endif
