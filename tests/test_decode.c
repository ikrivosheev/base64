#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "b64_stream.h"


#define ASSERT_EQ(expected, actual, message) \
    do { if(expected != actual) \
            { \
                printf("FAIL: "); \
                printf(message); \
                printf("\n"); \
                return true; \
            } \
    } while (0); \


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

    ASSERT_EQ(b64_stream_decode_final(&state), 0, "b64_stream_decode_final");
    ASSERT_EQ(expected_len, result_len, "Decoded result length is not equal expected");
    ASSERT_EQ(strncmp(result, expected, expected_len), 0, "Decoded result is not equal expected")
}

int test_decode() 
{
    bool fail = false;
    printf("START: DecodeTest simple\n");
    fail |= assert_decode("QUFB", "AAA", 3);
    printf("END: DecodeTest simple\n");
    printf("START: DecodeTest with padding\n");
    fail |= assert_decode("QUE=", "AA", 2);
    printf("END: DecodeTest with padding\n");
    printf("START: DecodeTest with big padding\n");
    fail |= assert_decode("QQ==", "A", 1);
    printf("END: DecodeTest with big padding\n");

    return fail;
}


int main()
{
    bool fail = false;
    fail |= test_decode();
    return fail ? 1 : 0;
}
