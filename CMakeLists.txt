cmake_minimum_required(VERSION 3.27)
project(logger LANGUAGES C)

add_library(shared-arena STATIC
        shared-arena/shared_arena.h
        shared-arena/shared_arena.c)
target_link_libraries(shared-arena PUBLIC rt)

add_library(interceptor SHARED
        interceptor/main.c
)
target_include_directories(interceptor PRIVATE shared-arena)
target_link_libraries(interceptor PRIVATE dl shared-arena)

add_executable(log-daemon
        log-daemon/main.c)
target_include_directories(log-daemon PRIVATE shared-arena)

add_executable(test-stand
        test-stand/main.c)

add_dependencies(test-stand interceptor)