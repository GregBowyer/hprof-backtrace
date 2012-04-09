CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE_DIRECTORIES(./include ../libpack/include)

ADD_EXECUTABLE(hprof-backtrace
    src/hprof_backtrace.c
    src/skiplist.c
)

TARGET_LINK_LIBRARIES(hprof-backtrace pack)
