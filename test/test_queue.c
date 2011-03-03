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
#include "../src/queue.h"

#include <stdlib.h>
#include <stdio.h>

static queue_t *priv_test_queue;

static void test_queue_null_init(void)
{
    priv_test_queue = NULL;
}

static void test_queue_null_cleanup(void)
{
    queue_destroy(priv_test_queue);
}

static void test_queue_empty_init(void)
{
    priv_test_queue = queue_create();
}

static void test_queue_empty_cleanup(void)
{
    queue_destroy(priv_test_queue);
}

static void test_queue_one_item_init(void)
{
    priv_test_queue = queue_create();
    queue_push(priv_test_queue, (void*) 1000);
}

static void test_queue_one_item_cleanup(void)
{
    queue_destroy(priv_test_queue);
}

static void test_queue_null(void)
{
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_next(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_last(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_previous(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_push(priv_test_queue, (void*) 1000));
    TEST_ASSERT_EQUAL(NULL, queue_pop(priv_test_queue));
    TEST_ASSERT_EQUAL(NULL, queue_get_current(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_remove_current(priv_test_queue));
}

static void test_queue_empty(void)
{
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_LAST, queue_next(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_last(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_LAST, queue_previous(priv_test_queue));
}

static void test_queue_not_null(void)
{
    TEST_ASSERT_NOT_EQUAL(QUEUE_NULL, queue_first(priv_test_queue));
    TEST_ASSERT_NOT_EQUAL(QUEUE_NULL, queue_next(priv_test_queue));
    TEST_ASSERT_NOT_EQUAL(QUEUE_NULL, queue_last(priv_test_queue));
    TEST_ASSERT_NOT_EQUAL(QUEUE_NULL, queue_previous(priv_test_queue));
}

static void test_queue_not_empty(void)
{
    TEST_ASSERT_NOT_EQUAL(QUEUE_EMPTY, queue_first(priv_test_queue));
    TEST_ASSERT_NOT_EQUAL(QUEUE_LAST, queue_next(priv_test_queue));
    TEST_ASSERT_NOT_EQUAL(QUEUE_EMPTY, queue_last(priv_test_queue));
    TEST_ASSERT_NOT_EQUAL(QUEUE_LAST, queue_previous(priv_test_queue));
}

static void test_queue_one_item(void)
{
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_push(priv_test_queue, (void*) 1000));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(1000, queue_get_current(priv_test_queue));
}

void test_queue(void)
{
    TEST_CASE_START();

    /* Test when the queue is null. */
    TEST_CASE_RUN(test_queue_null_init,
                  test_queue_null_cleanup,
                  test_queue_null);

    /* Test when the queue is empty. */
    TEST_CASE_RUN(test_queue_empty_init,
                  test_queue_empty_cleanup,
                  test_queue_empty);

    /* Test when the queue is not empty. */
    TEST_CASE_RUN(test_queue_one_item_init,
                  test_queue_one_item_cleanup,
                  test_queue_not_null);

    TEST_CASE_RUN(test_queue_one_item_init,
                  test_queue_one_item_cleanup,
                  test_queue_not_empty);

    /* Test to put one item into a queue. */
    TEST_CASE_RUN(test_queue_empty_init,
                  test_queue_empty_cleanup,
                  test_queue_one_item);

    TEST_CASE_END();
}
