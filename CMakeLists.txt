cmake_minimum_required(VERSION 3.5)

project(b64_stream VERSION 0.3.0)

option(B64_STREAM_BUILD_EXE "add executable" ON)
option(B64_STREAM_BUILD_TESTS "add test projects" ON)

add_library(b64_stream 
    STATIC
    # library files
    lib/decode.c
    lib/encode.c
)

set_target_properties(b64_stream 
    PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS OFF
    PUBLIC_HEADER include/b64_stream.h
)

target_include_directories(b64_stream PUBLIC .)

if (B64_STREAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if (B64_STREAM_BUILD_EXE)
    add_executable(base64 bin/base64.c)

    target_link_libraries(base64 b64_stream)
endif()

install(
    TARGETS b64_stream
    ARCHIVE
        DESTINATION static
    RUNTIME
        DESTINATION bin
    PUBLIC_HEADER
        DESTINATION include
)
