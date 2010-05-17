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
#include "queue.h"

#include <stdlib.h>

/*! Used for indicate if a slot is empty. */
#define SLOT_TYPE_EMPTY 0u
/*! Used for indicate if a slot contains one data item with a unique index in
 *  the hash lookup table. */
#define SLOT_TYPE_DATA 1u
/*! Used for indicate if a slot has a queue in just to be able to handle more
 *  than one data item with the same index in the hash lookup table. */
#define SLOT_TYPE_QUEUE 2u

typedef struct hash_data_t {
    /*! Contains the hash key which will be useful if the data item which has
     *  a single unique index gets another data item with the same index. In
     *  other words, the single data item needs to be converted to a queue of
     *  data items and therefore it is useful to be able to identify each
     *  data item by each hash key. */
    unsigned int key;
    /*! Contains a pointer to the data that is supposed to be looked up. */
    void * data;
} hash_data_t;

typedef struct hash_slot_t {
    unsigned int slot_type;
    union {
        hash_data_t *data;
        queue_t *queue;
    } slot;
} hash_slot_t;

hash_lookup_t * hash_lookup_create(unsigned int size)
{
    unsigned int i;

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
            this_ptr->slots[i].slot_type = SLOT_TYPE_EMPTY;
        }
    }
    return this_ptr;
}

int hash_lookup_insert(hash_lookup_t *this_ptr, unsigned int key, void * data)
{
    unsigned int index = key % this_ptr->slot_size;
    hash_data_t *new_data = (hash_data_t*) malloc(sizeof(hash_data_t));
    hash_data_t *temp;

    if (new_data != NULL) {
        new_data->data = data;
        new_data->key = key;

        switch (this_ptr->slots[index].slot_type) {
            case SLOT_TYPE_DATA:
                /* Create queue. */
                temp = this_ptr->slots[index].slot.data;
                this_ptr->slots[index].slot.queue = queue_create();
                this_ptr->slots[index].slot_type = SLOT_TYPE_QUEUE;
                queue_push(this_ptr->slots[index].slot.queue, temp);
                queue_push(this_ptr->slots[index].slot.queue, new_data);
                break;

            case SLOT_TYPE_QUEUE:
                /* Insert into queue. */
                queue_push(this_ptr->slots[index].slot.queue, new_data);
                break;

            case SLOT_TYPE_EMPTY:
                /* The key creates a unique index. */
                this_ptr->slots[index].slot.data = new_data;
                this_ptr->slots[index].slot_type = SLOT_TYPE_DATA;
                break;

            default:
                free(new_data);
                /* TODO: return error code. */
                break;
        }
    }
    return 0;
}

void * hash_lookup_remove(hash_lookup_t *this_ptr, unsigned int key)
{
    unsigned int index = key % this_ptr->slot_size;
    void *data = NULL;

    switch (this_ptr->slots[index].slot_type) {
        case SLOT_TYPE_DATA:
            data = this_ptr->slots[index].slot.data->data;
            free(this_ptr->slots[index].slot.data);
            this_ptr->slots[index].slot_type = SLOT_TYPE_EMPTY;

            break;

        case SLOT_TYPE_QUEUE:
            /* Remove from queue. */
            /* TODO: Implement. */
            break;

        default:
        case SLOT_TYPE_EMPTY:
            /* Do nothing. */
            break;
    }
    return data;
}

void * hash_lookup_find(hash_lookup_t *this_ptr, unsigned int key)
{
    unsigned int index = key % this_ptr->slot_size;
    void *data = NULL;

    switch (this_ptr->slots[index].slot_type) {
        case SLOT_TYPE_DATA:
            data = this_ptr->slots[index].slot.data->data;
            break;

        case SLOT_TYPE_QUEUE:
            /* Find from queue. */
            /* TODO: Implement. */
            break;

        default:
        case SLOT_TYPE_EMPTY:
            /* Do nothing. */
            break;
    }
}

void hash_lookup_destroy(hash_lookup_t *this_ptr)
{
    hash_data_t *temp;
    unsigned int i;

    /* Set all the allocated slots to empty. */
    for (i = 0; i < this_ptr->slot_size; i++) {

        switch(this_ptr->slots[i].slot_type) {
            case SLOT_TYPE_DATA:
                free(this_ptr->slots[i].slot.data);
                break;

            case SLOT_TYPE_QUEUE:
                while ((temp = queue_pop(this_ptr->slots[i].slot.queue))
                        != NULL) {

                    free(temp);
                }
                queue_destroy(this_ptr->slots[i].slot.queue);
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
