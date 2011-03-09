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

#include "queue.h"
#include "observer.h"
#include "subject.h"

#include <stdlib.h>

/*!
 * Creates and initializes an observer.
 *
 * \return A newly created observer.
 */
observer_t * observer_create(
        void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg))
{
    observer_t * this_ptr = (observer_t*) malloc(sizeof(observer_t));
    observer_init(this_ptr, notify);
    return this_ptr;
}

/*!
 * Initializes an observer.
 */
void observer_init(observer_t *this_ptr,
        void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg))
{
    observer_set_notify(this_ptr, notify);
    pthread_mutex_init(&this_ptr->mutex, NULL);
}

void observer_notify(observer_t *this_ptr, struct subject_t *from, void *msg)
{
    pthread_mutex_lock(&this_ptr->mutex);
    if (this_ptr->notify != NULL) {
        this_ptr->notify(this_ptr, from, msg);
    }
    pthread_mutex_unlock(&this_ptr->mutex);
}

void observer_set_notify(observer_t *this_ptr,
        void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg))
{
    this_ptr->notify = notify;
}

/*!
 * Deinitializes an observer.
 */
void observer_deinit(observer_t *this_ptr)
{
    pthread_mutex_destroy(&this_ptr->mutex);
}


void observer_destroy(observer_t *this_ptr)
{
    observer_deinit(this_ptr);
    free(this_ptr);
    this_ptr = NULL;
}
