/*
    Copyright (c) 2013, Peter Johansson <peter.johansson@gmx.com>

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
#include "../src/observer.h"
#include "../src/subject.h"

#include <stdlib.h>
#include <stdio.h>

static observer_t *priv_test_observer;

static observer_t *priv_test_notify_observer;
static subject_t *priv_test_notify_subject;
static void *priv_test_notify_msg;

static void test_notify(observer_t *this_ptr, struct subject_t *from, void *msg)
{
    priv_test_notify_observer = this_ptr;
    priv_test_notify_subject = from;
    priv_test_notify_msg = msg;
}

static void test_observer_null_init(void)
{
    priv_test_observer = NULL;
}

static void test_observer_null_cleanup(void)
{
    observer_destroy(priv_test_observer);
}

static void test_observer_notify_init(void)
{
    priv_test_observer = observer_create(test_notify);
}

static void test_observer_notify_cleanup(void)
{
    observer_destroy(priv_test_observer);
}

static void test_observer_notify_null_init(void)
{
    priv_test_observer = observer_create(NULL);
}

static void test_observer_notify_null_cleanup(void)
{
    observer_destroy(priv_test_observer);
}

static void test_observer_null(void)
{
    observer_init(priv_test_observer, NULL);
    TEST_ASSERT_EQUAL(OBSERVER_NULL, observer_set_notify(priv_test_observer,
                      NULL));
    TEST_ASSERT_EQUAL(OBSERVER_NULL, observer_notify(priv_test_observer,
                      NULL, NULL));
}

static void test_observer_notify(void)
{
    TEST_ASSERT_EQUAL(OBSERVER_SUCESS, observer_notify(priv_test_observer,
                      (subject_t*) 2, (void*) 5));
    TEST_ASSERT_EQUAL(priv_test_observer, priv_test_notify_observer);
    TEST_ASSERT_EQUAL(2, priv_test_notify_subject);
    TEST_ASSERT_EQUAL(5, priv_test_notify_msg);
}

static void test_observer_notify_null(void)
{
    priv_test_notify_observer = (observer_t*) 10;
    priv_test_notify_subject = (subject_t*) 11;
    priv_test_notify_msg = (void*) 12;

    TEST_ASSERT_EQUAL(OBSERVER_CALLBACK_NULL,
                      observer_notify(priv_test_observer,
                      (subject_t*) 4, (void*) 6));
    TEST_ASSERT_EQUAL(10, priv_test_notify_observer);
    TEST_ASSERT_EQUAL(11, priv_test_notify_subject);
    TEST_ASSERT_EQUAL(12, priv_test_notify_msg);
}

void test_observer(void)
{
    TEST_CASE_START();

    TEST_CASE_RUN(test_observer_null_init,
                  test_observer_null_cleanup,
                  test_observer_null);

    /* Test when the notify callback is registered. */
    TEST_CASE_RUN(test_observer_notify_init,
                  test_observer_notify_cleanup,
                  test_observer_notify);

    /* Test when the notify callback is not registered. */
    TEST_CASE_RUN(test_observer_notify_null_init,
                  test_observer_notify_null_cleanup,
                  test_observer_notify_null);

    TEST_CASE_END();
}
