#include <string.h>

#include "include/b64_stream.h"
#include "tests.h"

static bool assert_decode(
    const char** chunks,
    const char chunks_len,
    const char* expected, 
    const size_t expected_len) 
{
    int i;
    struct b64_decode_state state;
    char result[100]; 

    b64_stream_decode_init(&state);
    for (i = 0; i < chunks_len; i++)
    {
        b64_stream_decode(&state, chunks[i], strlen(chunks[i]), result);
    }

    ASSERT(b64_stream_decode_final(&state), "b64_stream_decode_final");
    ASSERT(expected_len == state.out_len, "Decoded result length is not equal expected");
    ASSERT(strncmp(result, expected, expected_len) == 0, "Decoded result is not equal expected");
    return true;
}

bool test_decode_simple()
{
    const char* chunks[] = {"QUFB"};
    return assert_decode(chunks, 1, "AAA", 3);
}

bool test_decode_padding()
{
    const char* chunks[] = {"QUE="};
    return assert_decode(chunks, 1, "AA", 2);
}

bool test_decode_big_padding()
{
    const char* chunks[] = {"QQ=="};
    return assert_decode(chunks, 1, "A", 1);
}

bool test_decode_with_newline()
{
    const char* chunks[] = {"Q\nQ=="};
    return assert_decode(chunks, 1, "A", 1);
}

int main()
{
    test_t tests[] = {
        TEST(decode_simple),
        TEST(decode_padding),
        TEST(decode_big_padding),
        TEST(decode_with_newline),
    };
    return RUN(tests);
}
