#include <string.h>

#include "b64_stream.h"
#include "tests.h"


static bool assert_encode(
    const char** chunks,
    const int chunks_len,
    const char* expected,
    const size_t expected_len
)
{
    int i;
    struct b64_encode_state state;
    char result[100];

    b64_stream_encode_init(&state);
    for (i = 0; i < chunks_len; i++)
    {
        b64_stream_encode(&state, chunks[i], strlen(chunks[i]), result);
    }
    
    ASSERT(b64_stream_encode_final(&state, result), "b64_stream_encode_final");
    ASSERT(expected_len == state.out_len, "Encoded result length is not equal expected");
    ASSERT(strncmp(result, expected, expected_len) == 0, "Decoded result is not equal expected");
    return true;
}

bool test_encode_simple()
{
    const char* chunks[] = {"AAA"};
    return assert_encode(chunks, 1, "QUFB", 4);
}

bool test_encode_with_padding()
{
    const char* chunks[] = {"AA"};
    return assert_encode(chunks, 1, "QUE=", 4);
}


int main()
{
    test_t tests[] = {
        TEST(encode_simple),
        TEST(encode_with_padding),
    };
    return RUN(tests);
}
