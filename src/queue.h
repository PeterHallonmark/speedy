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

#include <stdbool.h>

/*! The operation was successfully executed. */
#define QUEUE_SUCESS 0
/*! General error which mostly likely happens during malloc. */
#define QUEUE_ERROR -1
/*! Error code for when it is not possible to go further next or previous in
 *  the queue. */
#define QUEUE_LAST -2
/*! Error code for when the queue is empty. */
#define QUEUE_EMPTY -3
/*! Error code for when the queue doesn't exist. */
#define QUEUE_NULL -4

/*! A definition of the data pointer type. */
typedef void data_t;

struct node_t;


typedef struct queue_iterator_t {
    /*! The variable current is representing the current position in the
     *  queue. */
    struct node_t * current;
    /*! A boolean which keeps track if the previous iterator operation went to
     *  the next item or the previous item. */
    bool direction_next;
} queue_iterator_t;

typedef struct queue_t {
    struct node_t * first; /*!< The first item in the queue. */
    struct node_t * last; /*!< The last item in the queue. */
    /*! The iterator has been integrated to the queue since the queue is not
     * going to have two individual positions in the queue at the same time. */
    queue_iterator_t iterator;
} queue_t;

queue_t * queue_create(void);
void queue_init(queue_t * this_ptr);

data_t * queue_pop(queue_t *this_ptr);
int queue_push(queue_t *this_ptr, data_t* data);

int queue_first(queue_t *this_ptr);
int queue_last(queue_t *this_ptr);
int queue_next(queue_t *this_ptr);
int queue_previous(queue_t *this_ptr);
data_t * queue_get_current(queue_t * this_ptr);
int queue_remove_current(queue_t *this_ptr);

void queue_deinit(queue_t *this_ptr);
void queue_destroy(queue_t *this_ptr);
