add_executable(test_decode test_decode.c)
add_executable(test_encode test_encode.c)

target_link_libraries(test_decode
    b64_stream
)

target_link_libraries(test_encode
    b64_stream
)

add_test(test_decode test_decode)
add_test(test_encode test_encode)

