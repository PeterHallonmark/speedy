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

struct observer_t;
struct queue_t;

typedef struct subject_t {
    struct observer_t observer; /*!< C inheritance of the observer. */
    struct queue_t queue;
} subject_t;

/*!
 * Creates and initializes a subject.
 *
 * \return A pointer to the created subject.
 */
subject_t *subject_create(void);

/*!
 * Initializes a subject.
 */
subject_t *subject_init(subject_t *this_ptr);

/*!
 * Attached an observer to a subject.
 *
 * \return Error code.
 */
int subject_attach(subject_t *this_ptr, struct observer_t *observer);

/*!
 * Detach an observer from a subject.
 *
 * \return Error code.
 */
int subject_detach(subject_t *this_ptr, struct observer_t *observer);

void subject_notify(subject_t *this_ptr, void *arg);

void subject_deinit(subject_t *this_ptr);

void subject_destroy(subject_t *this_ptr);
