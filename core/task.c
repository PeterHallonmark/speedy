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
#include "hash_lookup.h"
#include "task_handler.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>

/*!
 * A struct which keeps track of the depencies.
 */
typedef struct task_dependency_t {
    unsigned int id; /*! Id of the dependency task. */
    char * name; /*!< The name of the dependency. */
    task_t *task; /*! The task which is a dependency. */
} task_dependency_t;

void task_notify(observer_t * observer, struct subject_t *from, void *msg);
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
task_t * task_create(struct service_t *service, struct task_handler_t *handler)
{
    if (service->name != NULL) {
        task_t * this_ptr = (task_t*) malloc(sizeof(task_t));

        if (this_ptr != NULL) {
            this_ptr->task_id = hash_generate(service->name());
            this_ptr->dependency_queue = NULL;
            this_ptr->service = service;
            this_ptr->task_handler = handler;
            this_ptr->counter = 0;
            subject_init((subject_t*) this_ptr);
            observer_set_notify((observer_t*) this_ptr, task_notify);

            /* Check if there is a provides string, if there isn't any provides
             * string, use the task name instead for the generated id. */
            if (service->provides != NULL) {
                this_ptr->provides_id = hash_generate(service->provides());
            } else {
                this_ptr->provides_id = this_ptr->task_id;
            }

            if (service->dependency != NULL) {
                char **dependency_arg = (char**) service->dependency();
                task_dependency_t *dependency;

                this_ptr->dependency_queue = queue_create();

                while (*dependency_arg != NULL) {
                    dependency = (task_dependency_t*) malloc(sizeof(
                                  task_dependency_t));

                    dependency->name = *dependency_arg;
                    dependency->id = hash_generate(dependency->name);
                    dependency->task = NULL;

                    queue_push(this_ptr->dependency_queue, dependency);

                    dependency_arg++;
                }
            }
        }
        return this_ptr;
    }
    return NULL;
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

    printf("%s\n",this_ptr->service->name());

    if (this_ptr->service->action != NULL) {
        if (this_ptr->service->action() < 0) {
            status = TASK_FAIL;
        }
    }
    subject_notify((subject_t*) this_ptr, (void*) status);

    return status;
}

/*!
 * Gets the task id.
 *
 * \param this_ptr - A pointer to the task.
 *
 * \return The task id.
 */
unsigned int task_get_id(task_t *this_ptr)
{
    return this_ptr->task_id;
}

/*!
 * Gets the task provides id.
 *
 * \param this_ptr - A pointer to the task.
 *
 * \return The task provides id.
 */
unsigned int task_get_provides_id(task_t *this_ptr)
{
    return this_ptr->provides_id;
}

/*!
 * Builds the dependency by using the observer pattern. All the dependencies are
 * observed.
 *
 * \param this_ptr - A pointer to the task.
 * \param lookup - A lookup table which contains all the tasks..
 */
int task_build_dependency(task_t *this_ptr, struct hash_lookup_t *lookup)
{
    if (this_ptr->dependency_queue != NULL) {
        task_dependency_t *dependency;
        task_t *task;

        queue_first(this_ptr->dependency_queue);
        while ((dependency = queue_get_current(this_ptr->dependency_queue)) !=
                NULL) {

            task = hash_lookup_find(lookup, dependency->id);
            if (task != NULL) {
                dependency->task = task;
                this_ptr->counter++;
                subject_attach((subject_t*) task, (observer_t*) this_ptr);
            }
            queue_next(this_ptr->dependency_queue);
        }
    }

    if (this_ptr->counter == 0) {
        task_handler_run_add_task(this_ptr->task_handler, this_ptr);
    }

    return 0;
}

/*!
 * Frees all the memory that was allocated during the creation and
 * deinitializes the task.
 *
 * \param this_ptr - A pointer to the task.
 */
void task_destroy(task_t *this_ptr)
{
    if (this_ptr != NULL) {
        if (this_ptr->dependency_queue != NULL) {
            task_dependency_t *dependency;

            while ((dependency = queue_pop(this_ptr->dependency_queue)) !=
                    NULL) {

                free(dependency);
            }
            queue_destroy(this_ptr->dependency_queue);
        }

        subject_deinit((subject_t*) this_ptr);
        free(this_ptr);
    }
}

/*!
 * Notifies the task that something has happened in one of the dependencies.
 *
 * \param observer - A pointer to the task when it acts as an observer.
 * \param from - The subject that sends the notify message.
 * \param msg - The actually message.
 */
void task_notify(observer_t * observer, struct subject_t *from, void *msg)
{
    task_t *this_ptr = (task_t*) observer;

    this_ptr->counter--;

    if (this_ptr->counter == 0) {
        task_handler_run_add_task(this_ptr->task_handler, this_ptr);
    }
}
