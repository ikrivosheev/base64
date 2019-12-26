#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "b64_stream.h"


#define TEST(func) {"test_" #func, test_ ## func}
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define RUN(tests) runner(tests, ARRAY_SIZE(tests))
#define ASSERT(condition, message) \
    do { \
        if(!condition) \
            { \
                printf("FAIL: "); \
                printf(message); \
                printf("\n"); \
                return false; \
            } \
    } while (0);

typedef bool (*test_func_t)(void);

typedef struct test
{
    char* name;
    test_func_t test;
    
} test_t;


static int runner(test_t tests[], size_t size) 
{
    bool success = true;
    for (int i = 0; i < size; i++)
    {
        printf("Test %s START\n", tests[i].name);
        bool test_success = tests[i].test();
        printf("Test %s %s\n", tests[i].name, test_success ? "SUCCESS" : "FAIL");
        success &= test_success;
    }
    return success ? 0 : 1;
}


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
