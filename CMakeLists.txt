cmake_minimum_required(VERSION 3.5)

project(b64_stream VERSION 0.0.1)

option(B64_STREAM_BUILD_TESTS "add test projects" ON)

add_library(b64_stream STATIC
    # library files
    lib/decode.c
    lib/encode.c
    include/b64_stream.h
)

set_target_properties(b64_stream PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS OFF
)

target_include_directories(b64_stream PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (B64_STREAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
