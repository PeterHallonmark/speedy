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
#include "task.h"
#include "thread_pool.h"

#include <stdio.h>
#include <stdlib.h>

void *thread_pool_run_thread(void *arg);

thread_pool_t *thread_pool_create(unsigned int threads)
{
    thread_pool_t *this_ptr = (thread_pool_t*) malloc(sizeof(thread_pool_t));
    int i;

    if (this_ptr != NULL) {
        this_ptr->threads = (pthread_t*) malloc(sizeof(pthread_t) * threads);
        this_ptr->condititon = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
        this_ptr->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
        this_ptr->queue = queue_create();

        if ((this_ptr->queue != NULL) && (this_ptr->threads != NULL) &&
            (this_ptr->condititon != NULL) && (this_ptr->mutex != NULL)) {

            this_ptr->continue_thread_pool = true;
            if (threads > 0) {
                this_ptr->thread_size = threads - 1;
            } else {
                this_ptr->thread_size = 0;
            }

            this_ptr->passive_threads = 0;
            this_ptr->tasks = 0;

            pthread_cond_init(this_ptr->condititon, NULL);
            pthread_mutex_init(this_ptr->mutex, NULL);

            for (i = 0; i < this_ptr->thread_size; i++) {
                pthread_create(&this_ptr->threads[i], NULL,
                               thread_pool_run_thread, this_ptr);
                printf("thread %d\n",i);
            }

        } else {
            free(this_ptr->threads);
            free(this_ptr->condititon);
            free(this_ptr->mutex);
            queue_destroy(this_ptr->queue);
            free(this_ptr);
            this_ptr = NULL;
        }
    }
    return this_ptr;
}

int thread_pool_wait(thread_pool_t *this_ptr)
{
    int i;

    printf("tasks: %d passive threads: %d\n",this_ptr->tasks, this_ptr->passive_threads);
    thread_pool_run_thread(this_ptr);

    for (i = 0; i < this_ptr->thread_size; i++) {
        pthread_join(this_ptr->threads[i], NULL);
    }
    return 0;
}

int thread_pool_exit(thread_pool_t *this_ptr)
{
    this_ptr->continue_thread_pool = false;
    pthread_cond_broadcast(this_ptr->condititon);
    return 0;
}

int thread_pool_add_task(thread_pool_t *this_ptr, task_t *task)
{
    int status;

    pthread_mutex_lock(this_ptr->mutex);
    this_ptr->tasks++;
    status = queue_push(this_ptr->queue, task);
    pthread_mutex_unlock(this_ptr->mutex);
    return status;
    return 0;
}

void *thread_pool_run_thread(void *arg)
{
    task_t *task;
    int i;

    thread_pool_t *this_ptr = (thread_pool_t*) arg;

    while (this_ptr->continue_thread_pool) {
        pthread_mutex_lock(this_ptr->mutex);

        task = NULL;
        task = queue_pop(this_ptr->queue);

        printf("tasks: %d passive threads: %d\n",this_ptr->tasks, this_ptr->passive_threads);

        if (this_ptr->passive_threads <= this_ptr->tasks) {
            for (i = 0; i < this_ptr->passive_threads; i++) {
                pthread_cond_signal(this_ptr->condititon);
                this_ptr->tasks--;
            }
        } else {
            for (i = 0; i < (this_ptr->tasks - 1); i++) {
                pthread_cond_signal(this_ptr->condititon);
                this_ptr->tasks--;
            }
        }

        if (task != NULL) {
            pthread_mutex_unlock(this_ptr->mutex);
            printf("run:  %s\n",task->service->get_name());
            task_run_initialization(task);

        } else {
            this_ptr->passive_threads++;

            if (this_ptr->passive_threads > this_ptr->thread_size) {
                this_ptr->continue_thread_pool = false;
                pthread_cond_broadcast(this_ptr->condititon);
                pthread_mutex_unlock(this_ptr->mutex);

            } else {
                pthread_cond_wait(this_ptr->condititon, this_ptr->mutex);
                pthread_mutex_unlock(this_ptr->mutex);

                pthread_mutex_lock(this_ptr->mutex);
                this_ptr->passive_threads--;
                pthread_mutex_unlock(this_ptr->mutex);
            }
        }
    }
    return 0;
}


void thread_pool_destroy(thread_pool_t *this_ptr)
{
    int i;

    if (this_ptr->continue_thread_pool) {
        thread_pool_exit(this_ptr);
    }

    for(i = 0; i < this_ptr->thread_size; i++) {
        pthread_detach(this_ptr->threads[i]);
    }

    pthread_mutex_destroy(this_ptr->mutex);
    pthread_cond_destroy(this_ptr->condititon);

    free(this_ptr->mutex);
    free(this_ptr->condititon);
    queue_destroy(this_ptr->queue);
    free(this_ptr->threads);
    free(this_ptr);
}
