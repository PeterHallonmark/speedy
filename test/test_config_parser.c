/*
    Copyright (c) 2010, Peter Johansson <zeronightfall@gmx.com>

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

static void test_init(void)
{
    priv_test_config = config_parser_open("test.conf");
}

static void test_cleanup(void)
{
    config_parser_close(priv_test_config);
}

static void test_run(void)
{
    TEST_ASSERT_NOT_EQUAL(priv_test_config, NULL);
    TEST_ASSERT_EQUAL(0, 1);
}

static void run2(void)
{
}

void test_config_parser(void)
{
    TEST_CASE_START();
    TEST_CASE_RUN(test_init, test_cleanup, test_run);
    TEST_CASE_RUN(NULL, NULL, run2);
    TEST_CASE_END();
}
