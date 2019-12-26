#ifndef _TESTS_H
#define _TESTS_H

#include <stdbool.h>

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


int runner(test_t tests[], size_t size) 
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

#endif // _TESTS_H
