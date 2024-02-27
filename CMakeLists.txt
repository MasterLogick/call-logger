cmake_minimum_required(VERSION 3.27)
project(logger LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(shared-arena STATIC
        shared-arena/SharedArena.h
        shared-arena/SharedArena.cpp)
target_link_libraries(shared-arena PUBLIC rt)

add_library(interceptor SHARED
        interceptor/main.cpp
)
target_include_directories(interceptor PRIVATE shared-arena)
target_link_libraries(interceptor PRIVATE dl shared-arena)

add_executable(log-daemon
        log-daemon/main.cpp
        log-daemon/ProgramArgs.cpp
        log-daemon/ProgramArgs.h
        log-daemon/LogReader.cpp
        log-daemon/LogReader.h
        log-daemon/LogPrinters.h
        log-daemon/LogPrinters.cpp
)
target_include_directories(log-daemon PRIVATE shared-arena)
target_link_libraries(log-daemon PRIVATE shared-arena)

add_executable(test-stand
        test-stand/main.c)

add_dependencies(test-stand interceptor)