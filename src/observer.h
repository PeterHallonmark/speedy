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

#ifndef _SPEEDY_OBSERVER_H_
#define _SPEEDY_OBSERVER_H_

#include <pthread.h>

struct subject_t;

/*! The operation was successfully executed. */
#define OBSERVER_SUCESS 0
/*! Error code for when the observer doesn't exist. */
#define OBSERVER_NULL -1
/*! Error code for when the callback function is null. */
#define OBSERVER_CALLBACK_NULL -2

/*!
 * A type definition of an observer object which is used to keep track of the
 * internal state of a specific observer.
 */
typedef struct observer_t {
    /*! A mutex which makes sure that only one subject is dealing with an
     *  observer. */
    pthread_mutex_t mutex;
    /*! A callback function which is exectuted when something has been
     *  observed. */
    void (*notify)(struct observer_t *this_ptr, struct subject_t *from,
                   void *msg);
} observer_t;

observer_t * observer_create(
       void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg));
void observer_init(observer_t *this_ptr,
       void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg));

int observer_notify(observer_t *this_ptr, struct subject_t *from, void *msg);
int observer_set_notify(observer_t *this_ptr,
     void (*notify)(observer_t *this_ptr, struct subject_t *from, void *msg));

void observer_deinit(observer_t *this_ptr);
void observer_destroy(observer_t *this_ptr);

#endif /* _SPEEDY_OBSERVER_H_ */
