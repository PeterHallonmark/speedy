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

#include "queue.h"
#include <stdlib.h>

typedef struct node_t {
    data_t *data;
    struct node_t *next;
} node_t;

/*!
 * Creates and initializes a queue.
 *
 * \return The created queue.
 */
queue_t * queue_create(void)
{
    queue_t * this_ptr = (queue_t*) malloc(sizeof(queue_t));
    queue_init(queue);
    return this_ptr;
}

/*!
 * Initializes a queue.
 *
 * \param this_ptr - A pointer to the queue
 */
void queue_init(queue_t *this_ptr)
{
    this_ptr->first = NULL;
    this_ptr->last = NULL;
    this_ptr->current = NULL;
}
/*!
 * Gets the next data item from the queue.
 *
 * \return A pointer to the data item which has been fetched from the queue.
 */
data_t * queue_pop(queue_t *this_ptr)
{
    node_t *node;
    data_t *data = NULL;

    this_ptr->current = NULL;

    if (this_ptr->first != NULL) {
        node = this_ptr->first;
        this_ptr->first = node->next;

        /* In case it was the last node. */
        if (this_ptr->first == NULL) {
            this_ptr->last = NULL;
        }

        data = node->data;
        free(node);
    }
    return data;
}

/*!
 *  Put the data item at the end of the queue.
 *
 * \param this_ptr - A pointer to the queue
 * \param data - A pointer to the data that is put on the queue.
 *
 * \return The created queue.
 */
int queue_push(queue_t *this_ptr, data_t* data)
{
    node_t * node = (node_t*) malloc(sizeof(node_t));

    node->data = data;
    node->next = NULL;

    this_ptr->current = NULL;

    if (this_ptr->last != NULL) {
        this_ptr->last->next = node;
        this_ptr->last = node;
    } else {
        /* The queue was empty so make the node both the first and the last. */
        this_ptr->first = node;
        this_ptr->last = node;
    }
}

/*!
 *  Set the internal iterator to the first item in the queue.
 *
 * \param this_ptr - A pointer to the queue
 *
 * \return status.
 */
int queue_first(queue_t *this_ptr)
{
    int status = QUEUE_LAST;

    if (this_ptr->first != NULL) {
        this_ptr->current = this_ptr->first;
        status = QUEUE_OK;
    }
    return status;
}

/*!
 *  Set the internal iterator to the next item in the queue.
 *
 * \param this_ptr - A pointer to the queue
 *
 * \return status.
 */
int queue_next(queue_t *this_ptr)
{
    int status = -QUEUE_LAST;

    if (this_ptr->current != NULL) {
        this_ptr->current = this_ptr->current->next;
        status = QUEUE_OK;
    }
    return status;
}
/*!
 *  Get the data at the current position by using the internal iterator.
 *
 * \param this_ptr - A pointer to the queue
 *
 * \return The data from the current position in the queue.
 */
data_t * queue_get_current(queue_t * this_ptr)
{
    data_t * data = NULL;

    if (this_ptr->current != NULL) {
        data = this_ptr->current->data;
    }
    return data;
}

/*!
 *  Deinitializes the queue.
 *
 * \param this_ptr - A pointer to the queue
 */
void queue_deinit(queue_t *this_ptr)
{
    while(queue_pop(this_ptr) != NULL) {
    }
}
/*!
 *  Remove the queue.
 *
 * \param this_ptr - A pointer to the queue
 */
void queue_destroy(queue_t *this_ptr)
{
    free(this_ptr);
}

