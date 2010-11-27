#include "unity/unity.h"

void setUp(void);
void tearDown(void);

typedef struct test_case_t {
    const char* file;
    unsigned int line;
    void (*init)(void);
    void (*clean)(void);
    void (*test)(void);
} test_case_t;


static void runTest(test_case_t *test_case)
{
    Unity.CurrentTestName = test_case->file;
    Unity.CurrentTestLineNumber = test_case->line;

    Unity.NumberOfTests++;

    if (TEST_PROTECT()) {
        if (test_case->init != NULL) {
            test_case->init();
        }
        if (test_case->test != NULL) {
            test_case->test();
        }
    }
    if (TEST_PROTECT() && !TEST_IS_IGNORED) {
        if (test_case->clean != NULL) {
            test_case->clean();
        }
    }
    UnityConcludeTest();
}

void setUp(void)
{
}

void tearDown(void)
{
}

void run(void)
{
    TEST_ASSERT(0);
    TEST_ASSERT_EQUAL(0, 1);
}

void run2(void)
{
}

int main(void)
{
    test_case_t test_case;
    test_case_t test_case2;


    UnityBegin();
    
    test_case.file = __FILE__;
    test_case.line = __LINE__;
    test_case.clean = tearDown;
    test_case.init = setUp;
    test_case.test = run;

    test_case2.file = __FILE__;
    test_case2.line = __LINE__;
    test_case2.clean = tearDown;
    test_case2.init = setUp;
    test_case2.test = run2;

    runTest(&test_case);
    runTest(&test_case2);


    UnityEnd();
    return 0;
}
