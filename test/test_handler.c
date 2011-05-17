/*
    Copyright (c) 2011, Peter Johansson <zeronightfall@gmx.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "test_handler.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char *priv_path = NULL;

void test_handler_run_test(test_case_t *test_case)
{
    Unity.CurrentTestName = test_case->name;
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

const char* test_handler_get_current_path(void)
{
    return priv_path;
}

void test_handler_init(int argc, char *argv[])
{
    char *path = strdup(argv[0]);
    size_t size = pathconf(".", _PC_PATH_MAX);
    char *str_ptr;

    (void) argc;

    /* Change directory to where the current executable is located. */
    str_ptr = strrchr(path,'/');
    *str_ptr = '\0';
    chdir(path);
    *str_ptr = '/';
    free(path);

    /* Get the path to where the current executable is located. */
    if ((priv_path = malloc(size)) != NULL) {
        str_ptr = getcwd(priv_path, size);
    }

    UnityBegin();
}

void test_handler_deinit(void)
{
    UnityEnd();
    free(priv_path);
}
