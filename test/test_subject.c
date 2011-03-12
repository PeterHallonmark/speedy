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
#include <stdio.h>
#include "../src/queue.h"
#include "../src/observer.h"
#include "../src/subject.h"

static subject_t *priv_test_subject;
static observer_t *priv_test_observer;
static int priv_msg = 0;

void test_notify(observer_t *this_ptr, struct subject_t *from, void *msg)
{
    (void) this_ptr;
    (void) from;
    priv_msg = (int) msg;
}

static void test_hash_subject_null_init(void)
{
    priv_test_subject = NULL;
}

static void test_hash_subject_null_cleanup(void)
{
    subject_destroy(priv_test_subject);
}

static void test_hash_subject_init(void)
{
    priv_test_subject = subject_create();
}

static void test_hash_subject_cleanup(void)
{
    subject_destroy(priv_test_subject);
}

static void test_hash_subject_one_observer_init(void)
{
    priv_test_subject = subject_create();
    priv_test_observer = observer_create(&test_notify);
}

static void test_hash_subject_one_observer_cleanup(void)
{
    observer_destroy(priv_test_observer);
    subject_destroy(priv_test_subject);
}

static void test_hash_subject_null(void)
{
    subject_init(priv_test_subject);
    TEST_ASSERT_EQUAL(SUBJECT_NULL, subject_notify(priv_test_subject, NULL));
    TEST_ASSERT_EQUAL(SUBJECT_NULL, subject_attach(priv_test_subject, NULL));
    TEST_ASSERT_EQUAL(SUBJECT_NULL, subject_detach(priv_test_subject, NULL));
    subject_deinit(priv_test_subject);
}

static void test_hash_subject_null_observer(void)
{
    TEST_ASSERT_EQUAL(SUBJECT_MISSING_OBSERVER,
                      subject_attach(priv_test_subject, NULL));
    TEST_ASSERT_EQUAL(SUBJECT_MISSING_OBSERVER,
                      subject_detach(priv_test_subject, NULL));
}

static void test_hash_subject_no_observer(void)
{
    TEST_ASSERT_EQUAL(SUBJECT_SUCESS, subject_notify(priv_test_subject, NULL));
}

static void test_hash_subject_one_observer(void)
{
    TEST_ASSERT_EQUAL(SUBJECT_SUCESS,
                      subject_attach(priv_test_subject, priv_test_observer));
    TEST_ASSERT_EQUAL(SUBJECT_SUCESS,
                      subject_notify(priv_test_subject, (void*) 1012));
    TEST_ASSERT_EQUAL(1012, priv_msg);
    TEST_ASSERT_EQUAL(SUBJECT_SUCESS,
                      subject_notify(priv_test_subject, (void*) 2034));
    TEST_ASSERT_EQUAL(2034, priv_msg);
    TEST_ASSERT_EQUAL(SUBJECT_SUCESS,
                      subject_detach(priv_test_subject, priv_test_observer));
}

void test_subject(void)
{
    TEST_CASE_START();

    /* Test how it works when the subject is set to null. */
    TEST_CASE_RUN(test_hash_subject_null_init,
                  test_hash_subject_null_cleanup,
                  test_hash_subject_null);

    /* Test how it works when a null observer is used. */
    TEST_CASE_RUN(test_hash_subject_init,
                  test_hash_subject_cleanup,
                  test_hash_subject_null_observer);

    /* Test how it works when there are no observers. */
    TEST_CASE_RUN(test_hash_subject_init,
                  test_hash_subject_cleanup,
                  test_hash_subject_no_observer);


    /* Test how it works when there is just one observer. */
    TEST_CASE_RUN(test_hash_subject_one_observer_init,
                  test_hash_subject_one_observer_cleanup,
                  test_hash_subject_one_observer);

    TEST_CASE_END();
}
