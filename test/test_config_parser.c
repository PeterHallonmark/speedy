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

config_parser_t *priv_test_config;

#define GET_NAMESPACE() config_parser_get_namespace(priv_test_config)
#define GET_COMMAND() config_parser_get_command(priv_test_config)
#define GET_ARGUMENT_SIZE() config_parser_get_argument_size(priv_test_config)
#define GET_NEXT_ARGUMENT() config_parser_get_next_argument(priv_test_config)
#define PARSE_FILE() config_parser_read(priv_test_config)

static void test_config_parser_missing_file_init(void)
{
    priv_test_config = config_parser_open(TEST_CASE_PATH "config_parser_0.txt");
}

static void test_config_parser_missing_file_cleanup(void)
{
    config_parser_close(priv_test_config);
}

static void test_config_parser_empty_file_init(void)
{
    priv_test_config = config_parser_open(TEST_CASE_PATH "config_parser_1.txt");
}

static void test_config_parser_empty_file_cleanup(void)
{
    config_parser_close(priv_test_config);
}

static void test_config_parser_init(void)
{
    priv_test_config = config_parser_open(TEST_CASE_PATH "config_parser_2.txt");
}

static void test_config_parser_cleanup(void)
{
    config_parser_close(priv_test_config);
}

static void test_config_parser_missing_file_run(void)
{
    TEST_ASSERT_NULL(priv_test_config);
    TEST_ASSERT_TRUE(config_parser_is_eof(priv_test_config));
    config_parser_set_namespace(priv_test_config, "test0");
    TEST_ASSERT_NULL(config_parser_get_namespace(priv_test_config));
    TEST_ASSERT_NULL(config_parser_get_command(priv_test_config));
    TEST_ASSERT_NULL(config_parser_get_error_msg(priv_test_config));
    TEST_ASSERT_EQUAL(0, GET_ARGUMENT_SIZE());
    TEST_ASSERT_EQUAL(PARSER_MISSING_FILE, PARSE_FILE());
}

static void test_config_parser_empty_file_run(void)
{
    TEST_ASSERT_NOT_NULL(priv_test_config);

    TEST_ASSERT_EQUAL_STRING("", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("", GET_COMMAND());
    config_parser_set_namespace(priv_test_config, "test1");
    TEST_ASSERT_EQUAL_STRING("test1", GET_NAMESPACE());
    TEST_ASSERT_NULL(config_parser_get_error_msg(priv_test_config));
    TEST_ASSERT_EQUAL(PARSER_NO_DATA, PARSE_FILE());
    TEST_ASSERT_TRUE(config_parser_is_eof(priv_test_config));
}

static void test_config_parser_run(void)
{
    TEST_ASSERT_NOT_EQUAL(NULL, priv_test_config);

    /* The first command should be command0 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example1", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command0", GET_COMMAND());
    TEST_ASSERT_EQUAL(0, GET_ARGUMENT_SIZE());
    TEST_ASSERT_NULL(GET_NEXT_ARGUMENT());

    /* The next command should be command1 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example1", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command1", GET_COMMAND());
    TEST_ASSERT_EQUAL(1, GET_ARGUMENT_SIZE());
    TEST_ASSERT_EQUAL_STRING("test1", GET_NEXT_ARGUMENT());
    TEST_ASSERT_NULL(GET_NEXT_ARGUMENT());

    /* The next command should be command2 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example1", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command2", GET_COMMAND());
    TEST_ASSERT_EQUAL(2, GET_ARGUMENT_SIZE());
    TEST_ASSERT_EQUAL_STRING("test2", GET_NEXT_ARGUMENT());
    TEST_ASSERT_EQUAL_STRING("test3", GET_NEXT_ARGUMENT());
    TEST_ASSERT_NULL(GET_NEXT_ARGUMENT());

    /* The next command should be command3 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example2", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command3", GET_COMMAND());
    TEST_ASSERT_EQUAL(1, GET_ARGUMENT_SIZE());
    TEST_ASSERT_EQUAL_STRING("test4", GET_NEXT_ARGUMENT());
    TEST_ASSERT_NULL(GET_NEXT_ARGUMENT());

    /* The next command should be command4 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example3", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command4", GET_COMMAND());
    TEST_ASSERT_EQUAL(2, GET_ARGUMENT_SIZE());
    TEST_ASSERT_EQUAL_STRING("test5", GET_NEXT_ARGUMENT());
    TEST_ASSERT_EQUAL_STRING("test6", GET_NEXT_ARGUMENT());
    TEST_ASSERT_NULL(GET_NEXT_ARGUMENT());

    /* The next command should be command5 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example3", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command5", GET_COMMAND());
    TEST_ASSERT_EQUAL(1, GET_ARGUMENT_SIZE());
    TEST_ASSERT_EQUAL_STRING("A text message", GET_NEXT_ARGUMENT());
    TEST_ASSERT_NULL(GET_NEXT_ARGUMENT());

    /* The next command should be command6 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example3", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command6", GET_COMMAND());
    TEST_ASSERT_EQUAL(4, GET_ARGUMENT_SIZE());
    TEST_ASSERT_EQUAL_STRING("test7", GET_NEXT_ARGUMENT());
    TEST_ASSERT_EQUAL_STRING("test8", GET_NEXT_ARGUMENT());
    TEST_ASSERT_EQUAL_STRING("test9", GET_NEXT_ARGUMENT());
    TEST_ASSERT_EQUAL_STRING("test10", GET_NEXT_ARGUMENT());
    TEST_ASSERT_NULL(GET_NEXT_ARGUMENT());

    /* The next command should be command7 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example3", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command7", GET_COMMAND());
    TEST_ASSERT_EQUAL(0, GET_ARGUMENT_SIZE());

    /* The next command should be command8 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example3", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command8", GET_COMMAND());
    TEST_ASSERT_EQUAL(0, GET_ARGUMENT_SIZE());

    /* The next command should be command9 */
    TEST_ASSERT_EQUAL(PARSER_OK, PARSE_FILE());
    TEST_ASSERT_EQUAL_STRING("example3", GET_NAMESPACE());
    TEST_ASSERT_EQUAL_STRING("command9", GET_COMMAND());
    TEST_ASSERT_EQUAL(0, GET_ARGUMENT_SIZE());

    TEST_ASSERT_FALSE(config_parser_is_eof(priv_test_config));
    TEST_ASSERT_EQUAL(PARSER_NO_DATA, PARSE_FILE());
    TEST_ASSERT_TRUE(config_parser_is_eof(priv_test_config));
}

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
    TEST_CASE_RUN(test_config_parser_init,
                  test_config_parser_cleanup,
                  test_config_parser_run);

    TEST_CASE_END();
}
