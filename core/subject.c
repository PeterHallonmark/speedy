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

#include "observer.h"
#include "queue.h"
#include "subject.h"


#include <stdlib.h>

/*!
 * Creates and initializes a subject.
 *
 * \return A pointer to the created subject.
 */
subject_t *subject_create(void)
{
    subject_t *this_ptr = (subject_t*) malloc(sizeof(subject_t));
    subject_init(this_ptr);
    return this_ptr;
}
/*!
 * Initializes a subject.
 */
void subject_init(subject_t *this_ptr)
{
    queue_init(&this_ptr->queue);
    observer_init(&this_ptr->observer);
}

/*!
 * Attached an observer to a subject.
 *
 * \return Error code.
 */
int subject_attach(subject_t *this_ptr, observer_t *observer)
{
    return queue_push(&this_ptr->queue, (void*) observer);
}

/*!
 * Detach an observer from a subject.
 *
 * \return Error code.
 */
int subject_detach(subject_t *this_ptr, observer_t *observer)
{
    /* TODO: Need to detach observer. */
    return 0;
}

void subject_notify(subject_t *this_ptr, void *arg)
{
    observer_t *current;

    queue_first(&this_ptr->queue);
    while((current = queue_get_current(&this_ptr->queue)) != NULL) {
        observer_notify(current);
        queue_next(&this_ptr->queue);
    }
}

void subject_deinit(subject_t *this_ptr)
{
    queue_deinit(&this_ptr->queue);
    observer_deinit(&this_ptr->observer);
}

void subject_destroy(subject_t *this_ptr)
{
    subject_deinit(this_ptr);
    free(this_ptr);
}
