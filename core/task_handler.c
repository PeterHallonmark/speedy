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

#include "task_handler.h"
#include "hash_lookup.h"
#include "core_type.h"
#include "observer.h"
#include "queue.h"
#include "subject.h"
#include "task.h"
#include "thread_pool.h"

#include <stdlib.h>
#include <stdio.h>


task_handler_t * task_handler_create(void)
{
    task_handler_t *this_ptr = (task_handler_t*) malloc(sizeof(task_handler_t));

    if (this_ptr != NULL) {
        if (task_handler_init(this_ptr) != TASK_HANDLER_SUCCESS) {
            task_handler_deinit(this_ptr);
            free(this_ptr);
            this_ptr = NULL;
        }
    }
    return this_ptr;
}

int task_handler_init(task_handler_t * this_ptr)
{
    this_ptr->task_lookup = hash_lookup_create(64);
    this_ptr->tasks = queue_create();
    this_ptr->thread_pool = thread_pool_create(12, task_run_action);

    if ((this_ptr->tasks == NULL) || (this_ptr->tasks == NULL)) {
        task_handler_deinit(this_ptr);
        return TASK_HANDLER_FAIL;
    }

    return TASK_HANDLER_SUCCESS;
}

int task_handler_add_tasks(task_handler_t * this_ptr,
        struct service_t *services, unsigned int services_size)
{
    task_t *task = NULL;
    int tasks = 0;
    int i;
    for (i = 0; i < services_size; i++) {
        task = task_create(&services[i], this_ptr);

        if (task != NULL) {
            if (queue_push(this_ptr->tasks, task) != QUEUE_ERROR) {
                tasks++;
            } else {
                task_destroy(task);
            }
        }
    }
    if (tasks != services_size) {
        return TASK_HANDLER_FAIL;
    }
    return TASK_HANDLER_SUCCESS;
}

int task_handler_calculate_dependency(task_handler_t * this_ptr)
{
    unsigned int provides_id;
    unsigned int task_id;
    task_t *task;

    /* Build up the task lookup table. */
    queue_first(this_ptr->tasks);
    while ((task = queue_get_current(this_ptr->tasks)) != NULL) {
        provides_id = task_get_provides_id(task);
        task_id = task_get_id(task);

        hash_lookup_insert(this_ptr->task_lookup, task_id, task);
        if (provides_id != task_id) {
            hash_lookup_insert(this_ptr->task_lookup, provides_id, task);
        }
        queue_next(this_ptr->tasks);
    }

    /* Use the lookup table to connect all the dependencies for the tasks. */
    queue_first(this_ptr->tasks);
    while ((task = queue_get_current(this_ptr->tasks)) != NULL) {
        task_build_dependency(task, this_ptr->task_lookup);
        queue_next(this_ptr->tasks);
    }
    return 0;
}

int task_handler_wait(task_handler_t * this_ptr)
{
    thread_pool_wait(this_ptr->thread_pool);
    return 0;
}

void task_handler_run_add_task(task_handler_t *this_ptr, task_t *task)
{
    thread_pool_add_task(this_ptr->thread_pool, task);
}

void task_handler_deinit(task_handler_t * this_ptr)
{
    task_t *task;

    if (this_ptr->tasks != NULL) {
        while((task = queue_pop(this_ptr->tasks)) != NULL) {
            task_destroy(task);
        }
        queue_destroy(this_ptr->tasks);
    }
    thread_pool_destroy(this_ptr->thread_pool);
    hash_lookup_destroy(this_ptr->task_lookup);
}

void task_handler_destroy(task_handler_t * this_ptr)
{
    task_handler_deinit(this_ptr);
    free(this_ptr);
}
