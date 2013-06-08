/*
    Copyright (c) 2013, Peter Johansson <peter.johansson@gmx.com>

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
 * \param notify - A pointer to the callback function.
 *
 * \return A newly created observer.
 */
observer_t * observer_create(
        void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg))
{
    observer_t * this_ptr = (observer_t*) malloc(sizeof(observer_t));
    (void) observer_init(this_ptr, notify);
    return this_ptr;
}

/*!
 * Initializes an observer.
 *
 * \param this_ptr - A pointer to the observer.
 * \param notify - A pointer to the callback function.
 */
void observer_init(observer_t *this_ptr,
        void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg))
{
    if (this_ptr != NULL) {
        observer_set_notify(this_ptr, notify);
        pthread_mutex_init(&this_ptr->mutex, NULL);
    }
}


/*!
 * Sends a notify message to an observer.
 *
 * \param this_ptr - A pointer to the observer.
 * \param from - A pointer to where the message was sent from.
 * \param msg - The message that was sent.
 *
 * \return \c OBSERVER_SUCESS if it was possible to notify the observer.
 * \return \c OBSERVER_NULL if the observer doesn't exist.
 * \return \c OBSERVER_CALLBACK_NULL if the callback function doesn't exist.
 */
int observer_notify(observer_t *this_ptr, struct subject_t *from, void *msg)
{
    int status = OBSERVER_SUCESS;

    if (this_ptr != NULL) {

        pthread_mutex_lock(&this_ptr->mutex);
        if (this_ptr->notify != NULL) {
            this_ptr->notify(this_ptr, from, msg);
        } else {
            status = OBSERVER_CALLBACK_NULL;
        }

        pthread_mutex_unlock(&this_ptr->mutex);

    } else {
        status = OBSERVER_NULL;
    }
    return status;
}

/*!
 * Sets the callback function for the observer.
 *
 * \param this_ptr - A pointer to the observer.
 * \param notify - A pointer to the callback function.
 *
 * \return \c OBSERVER_SUCESS if it was possible to setup the notify
                              callback function.
 * \return \c OBSERVER_NULL if the observer doesn't exist.
 */
int observer_set_notify(observer_t *this_ptr,
        void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg))
{
    int status = OBSERVER_SUCESS;

    if (this_ptr != NULL) {
        this_ptr->notify = notify;
    } else {
        status = OBSERVER_NULL;
    }

    return status;
}

/*!
 * Deinitializes an observer.
 *
 * \param this_ptr - A pointer to the observer.
 */
void observer_deinit(observer_t *this_ptr)
{
    if (this_ptr != NULL) {
        pthread_mutex_destroy(&this_ptr->mutex);
    }
}

/*!
 * Deallocates and deinitializes an observer.
 *
 * \param this_ptr - A pointer to the observer.
 */
void observer_destroy(observer_t *this_ptr)
{
    observer_deinit(this_ptr);
    free(this_ptr);
    this_ptr = NULL;
}
