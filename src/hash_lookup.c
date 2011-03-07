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
    /*! Keeps track of if the slot is empty, if it has one data item or if there
     * are more than two data items in the slot. */
    unsigned int slot_type;
    union {
        /*! In most of the cases it will only be one data item for each index
         *  and this pointer is used to access the data item. */
        hash_data_t *data;
        /*! A pointer to the queue if there is several data items with the same
         *  calculated index for the lookup table. This makes it possible to
         *  store several items with the same index in the lookup table. */
        queue_t *queue;
    } slot; /*!< A slot can contain either a data pointer a queue which then
                 contains several data pointers, slot is a union which makes
                 it possible to use both but not at the same time. */
} hash_slot_t;

static int hash_lookup_queue_push(queue_t *this_ptr, hash_data_t* data);
static void * hash_lookup_queue_find(queue_t *this_ptr, unsigned int key);
static void * hash_lookup_queue_remove(queue_t *this_ptr, unsigned int key);

/*!
 *  Creates and initializes a hash lookup table with a specific size.
 *
 * \param size - The size of the hash lookup table that is going to be created.
 *
 * \return The created hash lookup table when it was possible to create it,
 *         \c NULL otherwise.
 */
hash_lookup_t * hash_lookup_create(unsigned int size)
{
    unsigned int i;

    hash_lookup_t *this_ptr = (hash_lookup_t*) malloc(sizeof(hash_lookup_t));
    this_ptr->slot_size = 0u;

    if (this_ptr != NULL) {
        /* Try to allocate the slots. */
        this_ptr->slots = (hash_slot_t*) malloc(size * sizeof(hash_slot_t));
        if (this_ptr->slots == NULL) {
            /* It wasn't possible so free the memory and return NULL. */
            free(this_ptr);
            return NULL;
        }
        this_ptr->slot_size = size;

        /* Set all the allocated slots to empty. */
        for (i = 0; i < this_ptr->slot_size; i++) {
            this_ptr->slots[i].slot_type = SLOT_TYPE_EMPTY;
        }
    }
    return this_ptr;
}

/*!
 * Inserts a data item into the hash lookup table.
 *
 * \param this_ptr - A pointer to the hash lookup table.
 * \param key - A key to be able to calculate where the data item should be
 *              stored.
 * \param data - A pointer to the data item that is going to be referenced.
 *
 * \note The current implementation does not handle several items with the same
 *       key, this can be a problem but is something for future implementations.
 *
 * \return \c HASH_LOOKUP_SUCESS if it was possible to insert an item,
 *         \c HASH_LOOKUP_ERROR otherwise.
 */

int hash_lookup_insert(hash_lookup_t *this_ptr, unsigned int key, void * data)
{
    int status = HASH_LOOKUP_EMPTY;

    if (this_ptr != NULL) {
        unsigned int index = key % this_ptr->slot_size;
        hash_data_t *new_data = (hash_data_t*) malloc(sizeof(hash_data_t));
        hash_data_t *temp;

        if (new_data != NULL) {
            new_data->data = data;
            new_data->key = key;
            status = HASH_LOOKUP_SUCESS;

            switch (this_ptr->slots[index].slot_type) {
                case SLOT_TYPE_DATA:
                    /* This is the second item with the same lookup table index
                     * so create a queue and store both in it. */
                    temp = this_ptr->slots[index].slot.data;
                    this_ptr->slots[index].slot.queue = queue_create();
                    this_ptr->slots[index].slot_type = SLOT_TYPE_QUEUE;
                    queue_push(this_ptr->slots[index].slot.queue, temp);
                    /* Use the internal wrapper push function just to be able to
                     * check so that all keys are unique in the queue. */
                    status = hash_lookup_queue_push(
                             this_ptr->slots[index].slot.queue, new_data);
                    break;

                case SLOT_TYPE_QUEUE:
                    /* The queue already contains more than two items with the
                     * same lookup table index so insert the new item into the
                     * queue and check that the key is unique. */
                    status = hash_lookup_queue_push(
                             this_ptr->slots[index].slot.queue, new_data);
                    break;

                case SLOT_TYPE_EMPTY:
                    /* The key creates a unique index for the lookup table. */
                    this_ptr->slots[index].slot.data = new_data;
                    this_ptr->slots[index].slot_type = SLOT_TYPE_DATA;
                    break;

                default:
                    /* It shouldn't be possible to get here but if it happens
                     * return an error code and free the newly allocated memory.
                     */
                    free(new_data);
                    status = HASH_LOOKUP_ERROR;
                    break;
            }
        }
    }
    return status;
}

/*!
 * Removes a data item from the hash lookup table.
 *
 * \param this_ptr - A pointer to the hash lookup table.
 * \param key - A key to be able to calculate where the data item is stored.
 *
 * \note The current implementation does not handle several items with the same
 *       key, this can be a problem but is something for future implementations.
 *
 * \return The data item if it was possible to remove it from the hash lookup
 *         table, \c NULL otherwise.
 */
void * hash_lookup_remove(hash_lookup_t *this_ptr, unsigned int key)
{
    void *data = NULL;

    if (this_ptr != NULL) {
        unsigned int index = key % this_ptr->slot_size;

        switch (this_ptr->slots[index].slot_type) {
            case SLOT_TYPE_DATA:
                data = this_ptr->slots[index].slot.data->data;
                free(this_ptr->slots[index].slot.data);
                this_ptr->slots[index].slot_type = SLOT_TYPE_EMPTY;
                break;

            case SLOT_TYPE_QUEUE:
                data = hash_lookup_queue_remove(this_ptr->slots[index].slot.queue,
                                                key);
                break;

            default:
            case SLOT_TYPE_EMPTY:
                /* Do nothing. */
                break;
        }
    }
    return data;
}

/*!
 * Finds a data item from the hash lookup table.
 *
 * \param this_ptr - A pointer to the hash lookup table.
 * \param key - A key to be able to calculate where the data item is stored.
 *
* \note The current implementation does not handle several items with the same
 *       key, this can be a problem but is something for future implementations.
 *
 * \return The data item if it was possible to find it from the hash lookup
 *         table, \c NULL otherwise.
 */
void * hash_lookup_find(hash_lookup_t *this_ptr, unsigned int key)
{
    void *data = NULL;

    if (this_ptr != NULL) {
        unsigned int index = key % this_ptr->slot_size;

        switch (this_ptr->slots[index].slot_type) {
            case SLOT_TYPE_DATA:
                data = this_ptr->slots[index].slot.data->data;
                break;

            case SLOT_TYPE_QUEUE:
                data = hash_lookup_queue_find(this_ptr->slots[index].slot.queue,
                                              key);
                break;

            default:
            case SLOT_TYPE_EMPTY:
                /* Do nothing. */
                break;
        }
    }
    return data;
}

/*!
 *  Removes and deinitializes a hash lookup table.
 *
 * \param this_ptr - A pointer to the hash lookup table.
 */
void hash_lookup_destroy(hash_lookup_t *this_ptr)
{
    if (this_ptr != NULL) {

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
}

/*!
 * Internal function which pushes an data item onto the queue if it has an
 * unique key. If it doesn't have an unique key the function will return an
 * error code.
 *
 * \param this_ptr - A pointer to the queue.
 * \param data - A pointer to the data that is going to be inserted into the
 *               queue.
 *
 * \return \c HASH_LOOKUP_SUCESS if the push operation was successful,
 *         \c HASH_LOOKUP_MULTIPLE_KEY_ERROR otherwise.
 */
static int hash_lookup_queue_push(queue_t *this_ptr, hash_data_t* data)
{
    /* Check if the key is unique. */
    if (hash_lookup_queue_find(this_ptr, data->key) != NULL) {
        free(data);
        return HASH_LOOKUP_MULTIPLE_KEY_ERROR;
    }
    queue_push(this_ptr, data);
    return HASH_LOOKUP_SUCESS;
}

/*!
 * Internal function which searches for an data item from the queue.
 *
 * \param this_ptr - A pointer to the queue.
 * \param key - A unique key which identifies the data item.
 *
 * \return If it was successful return the data item,
 *         otherwise return \c NULL.
 */
static void * hash_lookup_queue_find(queue_t *this_ptr, unsigned int key)
{
    hash_data_t *current;
    data_t *data = NULL;

    queue_first(this_ptr);

    while ((current = queue_get_current(this_ptr)) != NULL) {
        if (current->key == key) {
            data = current->data;
            queue_last(this_ptr);
        }
        queue_next(this_ptr);
    }
    return data;
}

/*!
 * Internal function which removes for an data item from the queue.
 *
 * \param this_ptr - A pointer to the queue.
 * \param key - A unique key which identifies the data item.
 *
 * \return If it was successful return the data item,
 *         otherwise return \c NULL.
 */
static void * hash_lookup_queue_remove(queue_t *this_ptr, unsigned int key)
{
    hash_data_t *current;
    data_t *data = NULL;

    queue_first(this_ptr);

    while ((current = queue_get_current(this_ptr)) != NULL) {
        if (current->key == key) {
            data = current->data;
            queue_remove_current(this_ptr);
            queue_last(this_ptr);
            free(current);
        }
        queue_next(this_ptr);
    }
    return data;
}

