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

struct hash_slot_t;

/*! The operation was successfully executed. */
#define HASH_LOOKUP_SUCESS 0
/*! General error which mostly likely happens during malloc. */
#define HASH_LOOKUP_ERROR -1
/*! At this point the hash lookup does not support multiple data items with the
 *  same key, if that happens this error is returned. */
#define HASH_LOOKUP_MULTIPLE_KEY_ERROR -2
/*! The hash lookup hasn't been created yet. */
#define HASH_LOOKUP_EMPTY -1

typedef struct hash_lookup_t {
    struct hash_slot_t * slots;
    unsigned int slot_size;
} hash_lookup_t;

hash_lookup_t * hash_lookup_create(unsigned int size);

int hash_lookup_insert(hash_lookup_t *this_ptr, unsigned int key, void * data);
void * hash_lookup_remove(hash_lookup_t *this_ptr, unsigned int key);
void * hash_lookup_find(hash_lookup_t *this_ptr, unsigned int key);

void hash_lookup_destroy(hash_lookup_t *this_ptr);
