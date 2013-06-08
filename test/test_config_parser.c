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
#include "../src/config_parser.h"

#include <stdlib.h>
#include <stdbool.h>

config_handler_t priv_handler;

bool priv_start = false;
bool priv_end = false;
int priv_counter = 0;

/*****************************************************************************/
/* Missing file */
/*****************************************************************************/

/* Non of these callbacks should be call for a missing file. */
void missing_file_start_callback(void *handler)
{
    TEST_FAIL();
}

void missing_file_end_callback(void *handler)
{
    TEST_FAIL();
}

void missing_file_namespace_callback(void *handler, const char *name)
{
    TEST_FAIL();
}

void missing_file_command_callback(void *handler, const char *command)
{
    TEST_FAIL();
}

void missing_file_argument_callback(void *handler, const char *argument)
{
    TEST_FAIL();
}

void missing_file_error_callback(void *handler, const char* filename, int line,
                                 const char *error_msg)
{
    TEST_FAIL();
}


static void test_config_parser_missing_file_init(void)
{
    priv_handler.func_start_config = &missing_file_start_callback;
    priv_handler.func_end_config = &missing_file_end_callback;
    priv_handler.func_namespace = &missing_file_namespace_callback;
    priv_handler.func_argument = &missing_file_argument_callback;
    priv_handler.func_command = &missing_file_command_callback;
    priv_handler.func_error = &missing_file_error_callback;
    priv_handler.handler = NULL;
}

static void test_config_parser_missing_file_cleanup(void)
{
    /* Do nothing. */
}

static void test_config_parser_missing_file_run(void)
{
    int result = config_parser_read_file(TEST_CASE_PATH "config_parser_0.txt",
                                         &priv_handler);
    TEST_ASSERT_EQUAL(PARSER_MISSING_FILE, result);
}

/*****************************************************************************/
/* Empty file
/*****************************************************************************/

/* Only start and end of these callbacks should be call for an empty file. */
void empty_file_start_callback(void *handler)
{
    priv_start = true;
}

void empty_file_end_callback(void *handler)
{
    priv_end = true;
}

void empty_file_namespace_callback(void *handler, const char *name)
{
    TEST_FAIL();
}

void empty_file_command_callback(void *handler, const char *command)
{
    TEST_FAIL();
}

void empty_file_argument_callback(void *handler, const char *argument)
{
    TEST_FAIL();
}

void empty_file_error_callback(void *handler, const char* filename, int line,
                               const char *error_msg)
{
    TEST_FAIL();
}


static void test_config_parser_empty_file_init(void)
{
    priv_start = false;
    priv_end = false;

    priv_handler.func_start_config = &empty_file_start_callback;
    priv_handler.func_end_config = &empty_file_end_callback;
    priv_handler.func_namespace = &empty_file_namespace_callback;
    priv_handler.func_argument = &empty_file_argument_callback;
    priv_handler.func_command = &empty_file_command_callback;
    priv_handler.func_error = &empty_file_error_callback;
    priv_handler.handler = NULL;
}

static void test_config_parser_empty_file_cleanup(void)
{
    /* Do nothing. */
}

static void test_config_parser_empty_file_run(void)
{
    int result = config_parser_read_file(TEST_CASE_PATH "config_parser_1.txt",
                                         &priv_handler);
    TEST_ASSERT_EQUAL(PARSER_OK, result);

    /* Check that the start and end of the config parser has been executed. */
    TEST_ASSERT_EQUAL(true, priv_start);
    TEST_ASSERT_EQUAL(true, priv_end);
}

/*****************************************************************************/
/* Normal file
/*****************************************************************************/

/* Only start and end of these callbacks should be call for an empty file. */
void normal_file_start_callback(void *handler)
{
    priv_start = true;
}

void normal_file_end_callback(void *handler)
{
    priv_end = true;
}

void normal_file_namespace_callback(void *handler, const char *name)
{
    switch(priv_counter) {
        case 0:
            TEST_ASSERT_EQUAL_STRING("example0", name);
            break;
        case 1:
            TEST_ASSERT_EQUAL_STRING("example1", name);
            break;
        case 8:
            TEST_ASSERT_EQUAL_STRING("example2", name);
            break;
        case 11:
            TEST_ASSERT_EQUAL_STRING("example3", name);
            break;
        case 17:
            TEST_ASSERT_EQUAL_STRING("example3", name);
            break;
        default:
            fprintf(stderr, "Counter: %d\n", priv_counter);
            TEST_FAIL();
            break;
    }
    priv_counter++;
}

void normal_file_command_callback(void *handler, const char *command)
{
    switch(priv_counter) {
        case 2:
            TEST_ASSERT_EQUAL_STRING("command0", command);
            break;
        case 3:
            TEST_ASSERT_EQUAL_STRING("command1", command);
            break;
        case 5:
            TEST_ASSERT_EQUAL_STRING("command2", command);
            break;
        case 9:
            TEST_ASSERT_EQUAL_STRING("command3", command);
            break;
        case 12:
            TEST_ASSERT_EQUAL_STRING("command4", command);
            break;
        case 15:
            TEST_ASSERT_EQUAL_STRING("command5", command);
            break;
        case 18:
            TEST_ASSERT_EQUAL_STRING("command6", command);
            break;
        case 23:
            TEST_ASSERT_EQUAL_STRING("command7", command);
            break;
        case 24:
            TEST_ASSERT_EQUAL_STRING("command8", command);
            break;
        case 25:
            TEST_ASSERT_EQUAL_STRING("command9", command);
            break;
        case 26:
            TEST_ASSERT_EQUAL_STRING("command10", command);
            break;
        case 29:
            TEST_ASSERT_EQUAL_STRING("command11", command);
            break;
        default:
            fprintf(stderr, "Counter: %d\n", priv_counter);
            TEST_FAIL();
            break;
    }
    priv_counter++;
}

void normal_file_argument_callback(void *handler, const char *argument)
{
    switch(priv_counter) {
        case 4:
            TEST_ASSERT_EQUAL_STRING("test1", argument);
            break;
        case 6:
            TEST_ASSERT_EQUAL_STRING("test2", argument);
            break;
        case 7:
            TEST_ASSERT_EQUAL_STRING("test3", argument);
            break;
        case 10:
            TEST_ASSERT_EQUAL_STRING("test4", argument);
            break;
        case 13:
            TEST_ASSERT_EQUAL_STRING("test5", argument);
            break;
        case 14:
            TEST_ASSERT_EQUAL_STRING("test6", argument);
            break;
        case 16:
            TEST_ASSERT_EQUAL_STRING("A text message", argument);
            break;
        case 19:
            TEST_ASSERT_EQUAL_STRING("test7", argument);
            break;
        case 20:
            TEST_ASSERT_EQUAL_STRING("test8", argument);
            break;
        case 21:
            TEST_ASSERT_EQUAL_STRING("test9", argument);
            break;
        case 22:
            TEST_ASSERT_EQUAL_STRING("test10", argument);
            break;
        case 27:
            TEST_ASSERT_EQUAL_STRING("test 11", argument);
            break;
        case 28:
            TEST_ASSERT_EQUAL_STRING("test 12", argument);
            break;
        case 30:
            TEST_ASSERT_EQUAL_STRING("test13", argument);
            break;
        case 31:
            TEST_ASSERT_EQUAL_STRING("test14", argument);
            break;
        case 32:
            TEST_ASSERT_EQUAL_STRING("test 15", argument);
            break;
        case 33:
            TEST_ASSERT_EQUAL_STRING("test 16", argument);
            break;
        default:
            fprintf(stderr, "Counter: %d\n", priv_counter);
            TEST_FAIL();
            break;
    }
    priv_counter++;
}

void normal_file_error_callback(void *handler, const char* filename, int line,
                               const char *error_msg)
{
    TEST_FAIL();
}


static void test_config_parser_normal_file_init(void)
{
    priv_start = false;
    priv_end = false;
    priv_counter = 0;

    priv_handler.func_start_config = &normal_file_start_callback;
    priv_handler.func_end_config = &normal_file_end_callback;
    priv_handler.func_namespace = &normal_file_namespace_callback;
    priv_handler.func_argument = &normal_file_argument_callback;
    priv_handler.func_command = &normal_file_command_callback;
    priv_handler.func_error = &normal_file_error_callback;
    priv_handler.handler = NULL;
}

static void test_config_parser_normal_file_cleanup(void)
{
    /* Do nothing. */
}

static void test_config_parser_normal_file_run(void)
{
    int result = config_parser_read_file(TEST_CASE_PATH "config_parser_2.txt",
                                         &priv_handler);
    TEST_ASSERT_EQUAL(PARSER_OK, result);

    /* Check that the start and end of the config parser has been executed. */
    TEST_ASSERT_EQUAL(true, priv_start);
    TEST_ASSERT_EQUAL(true, priv_end);

    /* Check that all the test cases has been executed. */
    TEST_ASSERT_EQUAL(34, priv_counter);
}

/*****************************************************************************/


void test_config_parser(void)
{

    TEST_CASE_START();

    /* Test case that checks what happens when the file is missing. */
    TEST_CASE_RUN(test_config_parser_missing_file_init,
                  test_config_parser_missing_file_cleanup,
                  test_config_parser_missing_file_run);

    /* Test case that checks what happens when the file is empty. */
    TEST_CASE_RUN(test_config_parser_empty_file_init,
                  test_config_parser_empty_file_cleanup,
                  test_config_parser_empty_file_run);

    /* Test case that checks what happens with a normal file. */
    TEST_CASE_RUN(test_config_parser_normal_file_init,
                  test_config_parser_normal_file_cleanup,
                  test_config_parser_normal_file_run);
#if 0
    /* Test case that checks what happens with a errornous file. */
    TEST_CASE_RUN(test_config_parser_errornous_file_init,
                  test_config_parser_errornous_file_cleanup,
                  test_config_parser_errornous_file_run);
#endif
    TEST_CASE_END();

}
