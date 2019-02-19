cmake_minimum_required(VERSION 3.1)

project(base64 LANGUAGES C VERSION 0.0.1)

############################
# Compilation options
option(BASE64_BUILD_TESTS "add test projects" ON)

############################
# Library
add_library(base64 STATIC
    lib/decode.c
)

set_target_properties(base64 PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS OFF
)


if (BASE64_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
