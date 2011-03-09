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

static void test_hash_subject_null_init(void)
{
    priv_test_subject = NULL;
}

static void test_hash_subject_null_cleanup(void)
{
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

void test_subject(void)
{
    TEST_CASE_START();

    TEST_CASE_RUN(test_hash_subject_null_init,
                  test_hash_subject_null_cleanup,
                  test_hash_subject_null);

    TEST_CASE_END();
}
