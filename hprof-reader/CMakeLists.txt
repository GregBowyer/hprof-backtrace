CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE_DIRECTORIES(./include)

ADD_EXECUTABLE(hprof-backtrace
    src/hprof_backtrace.c
    src/skiplist.c
)
ADD_DEPENDENCIES(hprof-backtrace libpack)
TARGET_LINK_LIBRARIES(hprof-backtrace pack)
