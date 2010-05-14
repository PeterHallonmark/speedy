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

#define QUEUE_OK 0
#define QUEUE_ERROR -1
#define QUEUE_LAST -2

typedef void data_t;

struct node_t;

typedef struct queue_t {
    struct node_t * first;
    struct node_t * last;
    struct node_t * current;
} queue_t;

/*!
 * Creates and initializes a queue.
 *
 * \return The created queue.
 */
queue_t * queue_create(void);

/*!
 * Initializes a queue.
 *
 * \param this_ptr - A pointer to the queue
 */
void queue_init(queue_t * this_ptr);

/*!
 * Gets the next data item from the queue.
 *
 * \return A pointer to the data item which has been fetched from the queue.
 */
data_t * queue_pop(queue_t *this_ptr);

/*!
 *  Put the data item at the end of the queue.
 *
 * \param this_ptr - A pointer to the queue
 * \param data - A pointer to the data that is put on the queue.
 *
 * \return The created queue.
 */
int queue_push(queue_t *this_ptr, data_t* data);

/*!
 *  Set the internal iterator to the first item in the queue.
 *
 * \param this_ptr - A pointer to the queue
 *
 * \return status.
 */
int queue_first(queue_t *this_ptr);

/*!
 *  Set the internal iterator to the next item in the queue.
 *
 * \param this_ptr - A pointer to the queue
 *
 * \return status.
 */
int queue_next(queue_t *this_ptr);

/*!
 *  Get the data at the current position by using the internal iterator.
 *
 * \param this_ptr - A pointer to the queue
 *
 * \return The data from the current position in the queue.
 */
data_t * queue_get_current(queue_t * this_ptr);

/*!
 *  Deinitializes the queue.
 *
 * \param this_ptr - A pointer to the queue
 */
void queue_deinit(queue_t *this_ptr);

/*!
 *  Remove the queue.
 *
 * \param this_ptr - A pointer to the queue
 */
void queue_destroy(queue_t *this_ptr);
