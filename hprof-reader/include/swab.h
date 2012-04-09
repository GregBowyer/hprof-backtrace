#ifndef _swab_h_
#define _swab_h_

#include <stdint.h>

extern inline uint16_t __swab16(uint16_t x) {
    return ((uint16_t) (
             (((uint16_t)(x) & (uint16_t)0x00FF) << 8) |
             (((uint16_t)(x) & (uint16_t)0xFF00) >> 8) ));
}

extern inline uint32_t __swab32(uint32_t x) {
    return ((uint32_t) (
             (((uint32_t)(x) & (uint32_t)0x000000FF) << 24) |
             (((uint32_t)(x) & (uint32_t)0x0000FF00) <<  8) |
             (((uint32_t)(x) & (uint32_t)0x00FF0000) >>  8) |
             (((uint32_t)(x) & (uint32_t)0xFF000000) >> 24) ));
}

extern inline uint64_t __swab64(uint64_t x) {
    return ((uint64_t) (
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000000000ff) << 56) |
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000000000ff00) << 40) |
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000000000ff0000) << 24) |
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000ff000000) <<  8) |
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000ff00000000) >>  8) |
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000ff0000000000) >> 24) |
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00ff000000000000) >> 40) |
        (uint64_t)(((uint64_t)(x) & (uint64_t)0xff00000000000000) >> 56) ));
}

extern inline int __is_big_endian(void) {
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}

extern inline int be16_to_native(uint16_t i) {
    if (!__is_big_endian()) {
        return __swab16(i);
    } else {
        return i;
    }
}

extern inline int be32_to_native(uint32_t i) {
    if (!__is_big_endian()) {
        return __swab32(i);
    } else {
        return i;
    }
}

extern inline int be64_to_native(uint64_t i) {
    if (!__is_big_endian()) {
        return __swab64(i);
    } else {
        return i;
    }
}

extern inline int le16_to_native(uint16_t i) {
    if (__is_big_endian()) {
        return __swab16(i);
    } else {
        return i;
    }
}

extern inline int le32_to_native(uint32_t i) {
    if (__is_big_endian()) {
        return __swab32(i);
    } else {
        return i;
    }
}

extern inline int le64_to_native(uint64_t i) {
    if (__is_big_endian()) {
        return __swab64(i);
    } else {
        return i;
    }
}

#endif 
