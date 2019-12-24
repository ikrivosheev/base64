add_executable(test_decode test_decode.c)

target_link_libraries(test_decode
    b64_stream
)

add_test(test_decode test_decode)

