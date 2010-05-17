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

struct hash_slot_t;

typedef struct hash_lookup_t {
    struct hash_slot_t * slots;
    unsigned int slot_size;
} hash_lookup_t;

hash_lookup_t * hash_lookup_create(unsigned int size);

int hash_lookup_insert(hash_lookup_t *this_ptr, unsigned int key, void * data);
void * hash_lookup_remove(hash_lookup_t *this_ptr, unsigned int key);
void * hash_lookup_find(hash_lookup_t *this_ptr, unsigned int key);

void hash_lookup_destroy(hash_lookup_t *this_ptr);
