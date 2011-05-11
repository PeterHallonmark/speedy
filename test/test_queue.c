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
#include "../src/queue.h"

#include <stdlib.h>
#include <stdio.h>

static queue_t *priv_test_queue;

static void test_queue_null_init(void)
{
    priv_test_queue = NULL;
    queue_init(priv_test_queue);
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
    queue_push(priv_test_queue, (void*) 1000u);
}

static void test_queue_one_item_cleanup(void)
{
    queue_destroy(priv_test_queue);
}

static void test_queue_two_items_init(void)
{
    priv_test_queue = queue_create();
    queue_push(priv_test_queue, (void*) 4001u);
    queue_push(priv_test_queue, (void*) 4002u);
}

static void test_queue_two_items_cleanup(void)
{
    queue_destroy(priv_test_queue);
}

static void test_queue_three_items_init(void)
{
    priv_test_queue = queue_create();
    queue_push(priv_test_queue, (void*) 5001u);
    queue_push(priv_test_queue, (void*) 5002u);
    queue_push(priv_test_queue, (void*) 5003u);
}

static void test_queue_three_items_cleanup(void)
{
    queue_destroy(priv_test_queue);
}

static void test_queue_null(void)
{
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_next(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_last(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_previous(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_NULL, queue_push(priv_test_queue, (void*) 1001u));
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
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_push(priv_test_queue, (void*) 1002u));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(1002u, queue_get_current(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_next(priv_test_queue));
    TEST_ASSERT_EQUAL(NULL, queue_get_current(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_LAST, queue_next(priv_test_queue));
}

static void test_queue_1000_items(void)
{
    unsigned int i;

    for (i=0u; i < 1000u; i++) {
        TEST_ASSERT_EQUAL(QUEUE_SUCESS,
                          queue_push(priv_test_queue, (void*) (2000u + i)));
    }
    for (i=0u; i < 1000u; i++) {
        TEST_ASSERT_EQUAL(2000u + i, queue_pop(priv_test_queue));
    }
    TEST_ASSERT_EQUAL(NULL, queue_pop(priv_test_queue));
}

static void test_queue_1000_items_v2(void)
{
    unsigned int i;

    for (i=0u; i < 500u; i++) {
        TEST_ASSERT_EQUAL(QUEUE_SUCESS,
                          queue_push(priv_test_queue, (void*) (3000u + i)));
    }
    for (i=0u; i < 200u; i++) {
        TEST_ASSERT_EQUAL(3000u + i, queue_pop(priv_test_queue));
    }
    for (i=0u; i < 500u; i++) {
        TEST_ASSERT_EQUAL(QUEUE_SUCESS,
                          queue_push(priv_test_queue, (void*) (3500u + i)));
    }
    for (i=0u; i < 800u; i++) {
        TEST_ASSERT_EQUAL(3200u + i, queue_pop(priv_test_queue));
    }
    TEST_ASSERT_EQUAL(NULL, queue_pop(priv_test_queue));
}

static void test_queue_remove_first_item(void)
{
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_remove_current(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_remove_current(priv_test_queue));
}

static void test_queue_remove_last_item(void)
{
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_last(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_remove_current(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_last(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_remove_current(priv_test_queue));
}

static void test_queue_remove_first_item_v2(void)
{
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_remove_current(priv_test_queue));
    test_queue_remove_first_item();
}

static void test_queue_remove_last_item_v2(void)
{
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_last(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_remove_current(priv_test_queue));
    test_queue_remove_last_item();
}

static void test_queue_remove_middle_item(void)
{
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_first(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_next(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_remove_current(priv_test_queue));
    TEST_ASSERT_EQUAL(QUEUE_SUCESS, queue_next(priv_test_queue));
    test_queue_remove_last_item_v2();
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

    /* Test when the queue is not null. */
    TEST_CASE_RUN(test_queue_one_item_init,
                  test_queue_one_item_cleanup,
                  test_queue_not_null);

    /* Test when the queue is not empty. */
    TEST_CASE_RUN(test_queue_one_item_init,
                  test_queue_one_item_cleanup,
                  test_queue_not_empty);

    /* Test to put one item into a queue. */
    TEST_CASE_RUN(test_queue_empty_init,
                  test_queue_empty_cleanup,
                  test_queue_one_item);

    /* Test to put 1000 items into a queue. */
    TEST_CASE_RUN(test_queue_empty_init,
                  test_queue_empty_cleanup,
                  test_queue_1000_items);

    /* Test to put 1000 items to the queue but pop 200 before the queue has
       reached 1000 items. */
    TEST_CASE_RUN(test_queue_empty_init,
                  test_queue_empty_cleanup,
                  test_queue_1000_items_v2);

    /* Remove first and only item. */
    TEST_CASE_RUN(test_queue_one_item_init,
                  test_queue_one_item_cleanup,
                  test_queue_remove_first_item);

    /* Remove last and only item. */
    TEST_CASE_RUN(test_queue_one_item_init,
                  test_queue_one_item_cleanup,
                  test_queue_remove_last_item);

    /* Remove first item when the queue contains 2 items. */
    TEST_CASE_RUN(test_queue_two_items_init,
                  test_queue_two_items_cleanup,
                  test_queue_remove_first_item_v2);

    /* Remove last item when the queue contains 2 items. */
    TEST_CASE_RUN(test_queue_two_items_init,
                  test_queue_two_items_cleanup,
                  test_queue_remove_last_item_v2);

    /* Remove middle item. */
    TEST_CASE_RUN(test_queue_three_items_init,
                  test_queue_three_items_cleanup,
                  test_queue_remove_middle_item);

    TEST_CASE_END();
}
