CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE_DIRECTORIES(./include)

ADD_LIBRARY(pack STATIC
    src/pack.c
    src/parse.c
    src/stream.c
    src/string.c
    src/unpack.c
)
