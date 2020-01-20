#include <string.h>

#include "b64_stream.h"
#include "tests.h"


static bool assert_encode(
    const char* src,
    const int src_len,
    const char* expected,
    const size_t expected_len
)
{
    struct b64_encode_state state;
    char result[100];

    b64_stream_encode_init(&state);
    b64_stream_encode(&state, src, src_len, result);
    
    ASSERT(b64_stream_encode_final(&state, result), "b64_stream_encode_final");
    ASSERT(expected_len == state.out_len, "Encoded result length is not equal expected");
    ASSERT(strncmp(result, expected, expected_len) == 0, "Decoded result is not equal expected");
    return true;
}

bool test_encode_simple()
{
    return assert_encode("AAA", 3, "QUFB", 4);
}

bool test_encode_with_padding()
{
    return assert_encode("AA", 2, "QUE=", 4);
}


int main()
{
    test_t tests[] = {
        TEST(encode_simple),
        TEST(encode_with_padding),
    };
    return RUN(tests);
}
