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

#include "core_type.h"
#include "observer.h"
#include "queue.h"
#include "subject.h"
#include "hash.h"
#include "task.h"

#include <stdlib.h>

/*!
 * Creates a task which encapsulates a service.
 * The reason for this is to make it possible to observe services and to track
 * dependencies. A task can be both an observer and a subject since it uses
 * C inheritance from the \c struct \c subject_t.
 *
 * \param service - A service that is going to be encapsulated into a task.
 *
 * \return A task which encapsulates a service.
 */
task_t * task_create(struct service_t *service)
{
    task_t * this_ptr = (task_t*) malloc(sizeof(task_t));

    if (this_ptr != NULL) {
        if (this_ptr->service->get_name != NULL) {
            this_ptr->task_id = hash_generate(service->get_name());
            this_ptr->provides = NULL;

            /* Check if 'provides' is the same as the name of the task. */
            if (this_ptr->service->provides != NULL) {
                this_ptr->provides = (unsigned int*) malloc(sizeof(
                                      unsigned int));
                *this_ptr->provides = hash_generate(service->provides());
                if (this_ptr->task_id == *this_ptr->provides) {
                    /* 'Provides' is the same as the name of the task so remove
                     * it since it should be enough with just the name. */
                    free(this_ptr->provides);
                    this_ptr->provides = NULL;
                }
            }

            subject_init(&this_ptr->task);
            this_ptr->service = service;

        } else {
            /* It wasn't possible to create a unique id for the task so remove
             * the task and free the allocated memory. */

            /* TODO Need to log it somehow to know if the hash_lookup table
             *      needs to be able to track different tasks with the same id.
             */
            free(this_ptr);
            this_ptr = NULL;
        }
    }
    return this_ptr;
}

/*!
 * Executes the initialization function from the service which basically is
 * only run at startup.
 *
 * \param this_ptr - A pointer to the task.
 *
 * \return TASK_SUCCESS if it was possible to execute the initialization
 *         function or if it didn't exist. It returns a negative value if there
 *         was an error detected during the execution of the initialization
 *         function.
 */
int task_run_initialization(task_t *this_ptr)
{
    int status = TASK_SUCCESS;

    if (this_ptr->service->initialization != NULL) {
        status = this_ptr->service->initialization();
    }
    return status;
}

/*!
 * Frees all the memory that was allocated during the creation and
 * deinitializes the task.
 *
 * \param this_ptr - A pointer to the task.
 */
void task_destroy(task_t *this_ptr)
{
    subject_deinit(&this_ptr->task);
    free(this_ptr->provides);
    free(this_ptr);
}
