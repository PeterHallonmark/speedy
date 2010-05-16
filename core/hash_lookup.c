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

#include "hash_lookup.h"
#include "hash.h"
#include "queue.h"

#define SLOT_TYPE_EMPTY 0
#define SLOT_TYPE_DATA 1
#define SLOT_TYPE_QUEUE 2

typedef struct hash_data_t {
    unsigned int hash_key;
    void * data;
} hash_data_t;

typedef struct hash_slot_t {
    unsigned int slot_type;
    union slot {
        hash_data_t *data;
        queue_t *queue;
    };
} hash_slot_t;

hash_lookup_t * hash_lookup_create(unsigned int size)
{
    int i = 0;

    hash_lookup_t *this_ptr = (hash_lookup_t*) malloc(sizeof(hash_lookup_t));

    if (this_ptr != NULL) {
        this_ptr->slot_size = 0u;

        /* Try to allocate the slots. */
        this_ptr->slots = (hash_slot_t*) malloc(size * sizeof(hash_slot_t));
        if (this_ptr->slots == NULL) {
            /* It wasn't possible so free the memory and return NULL. */
            free(this_ptr);
            return NULL;
        }

        /* Set all the allocated slots to empty. */
        for (i = 0; i < this_ptr->slot_size; i++) {
            this_ptr->slots[i]->slot_type = SLOT_TYPE_EMPTY;
        }
    }
    return this_ptr;
}


void hash_lookup_destroy(hash_lookup_t *this_ptr)
{
    hash_data_t *temp;

    /* Set all the allocated slots to empty. */
    for (i = 0; i < this_ptr->slot_size; i++) {

        switch(this_ptr->slots[i]->slot_type) {
            case SLOT_TYPE_DATA:
                free(this_ptr->slots[i]->data);
                break;

            case SLOT_TYPE_QUEUE:
                while ((temp = queue_pop(this_ptr->slots[i]->queue)) != NULL) {
                    free(temp);
                }
                queue_destroy(this_ptr->slots[i]->queue);
                break;

            case SLOT_TYPE_EMPTY:
            default:
                /* Do nothing. */
                break;
        }
    }
    free(this_ptr->slots);
    free(this_ptr);
}
