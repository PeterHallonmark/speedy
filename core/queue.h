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

#include <stdbool.h>

#define QUEUE_OK 0
#define QUEUE_ERROR -1
#define QUEUE_LAST -2
#define QUEUE_EMPTY -3

typedef void data_t;

struct node_t;

typedef struct queue_t {
    struct node_t * first;
    struct node_t * last;
    struct node_t * current;
    bool direction_next;
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
