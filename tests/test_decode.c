#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "b64_stream.h"
#include "tests.h"

static bool assert_decode(
    const char* src, 
    const char* expected, 
    const size_t expected_len) 
{
    struct b64_state state;
    char result[100]; 
    size_t result_len;

    b64_stream_decode_init(&state);
    b64_stream_decode(&state, src, 4, &result[0], &result_len);

    ASSERT(b64_stream_decode_final(&state), "b64_stream_decode_final");
    ASSERT(expected_len == result_len, "Decoded result length is not equal expected");
    ASSERT(strncmp(result, expected, expected_len) == 0, "Decoded result is not equal expected");
    return true;
}

bool test_decode_simple()
{
    return assert_decode("QUFB", "AAA", 3);
}

bool test_decode_padding()
{
    return assert_decode("QUE=", "AA", 2);
}

bool test_decode_big_padding()
{
    return assert_decode("QQ==", "A", 1);
}

int main()
{
    test_t tests[] = {
        TEST(decode_simple),
        TEST(decode_padding),
        TEST(decode_big_padding),
    };
    return RUN(tests);
}
