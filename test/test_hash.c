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
#include "../src/hash.h"

#include <stdlib.h>
#include <stdio.h>

static void test_hash_generate_key(void)
{
    /* Test that the hash function generates the same hash key for a similar
     * text. */
    TEST_ASSERT_EQUAL(hash_generate("key"), hash_generate("key"));
    TEST_ASSERT_EQUAL(hash_generate("key1"), hash_generate("key1"));
    TEST_ASSERT_EQUAL(hash_generate("key12345"), hash_generate("key12345"));

    /* Test that the hash function generates a different key for a almost
     * identical text. */
    TEST_ASSERT_NOT_EQUAL(hash_generate("ky"), hash_generate("key"));
    TEST_ASSERT_NOT_EQUAL(hash_generate("key12"), hash_generate("key1"));
    TEST_ASSERT_NOT_EQUAL(hash_generate("key12345"), hash_generate("key12346"));
}

void test_hash(void)
{
    TEST_CASE_START();

    TEST_CASE_RUN(NULL, NULL, test_hash_generate_key);

    TEST_CASE_END();
}
