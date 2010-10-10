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
#include "task_parser.h"
#include "config_parser.h"
#include "task_handler.h"
#include "thread_pool.h"

#include <stdlib.h>
#include <stdio.h>

/*! Successful return code for thread pool callback function. */
#define TASK_PARSER_EXEC_SUCCESS 0

/*!
 * A simpler structure for tasks which doesn't have dependencies.
 */
typedef struct task_parser_task_t {
    /*!< A pointer to the task parser handle. */
    task_parser_t *this_ptr;
    /*! A pointer to the functional call for the task. */
    void (*task)(void *arg);
    /*! Arguments for the task function. */
    void *arg;
} task_parser_task_t;


static int task_parser_exec(void *task);
static void task_parser_read_file(void *arg);

/*!
 * Creates a task parser handle.
 *
 * \param this_ptr - A pointer to the task handler where all the tasks should
 *                   be registered.
 *
 * \return A pointer to the task parser handle if it was successfully created,
           \c NULL otherwise.
 */
task_parser_t* task_parser_create(task_handler_t *handler)
{
    task_parser_t *task_parser = malloc(sizeof(task_parser_t));
    
    if (task_parser != NULL) {
        task_parser->handler = handler;
        task_parser->thread_pool = thread_pool_create(4, task_parser_exec);

        if (task_parser->thread_pool == NULL) {
            free(task_parser);
            task_parser = NULL;
        }
    }
    return task_parser;
}

/*!
 * Creates a task which reads a config file.
 *
 * \param this_ptr - A pointer to the task parser handle.
 * \param filename - The config file.
 */
void task_parser_read(task_parser_t *this_ptr, const char * filename)
{
    task_parser_task_t *task_parser_task = malloc(sizeof(task_parser_task_t));

    task_parser_task->task = task_parser_read_file;
    task_parser_task->this_ptr = this_ptr;
    task_parser_task->arg = (void*) filename;

    thread_pool_add_task(this_ptr->thread_pool, task_parser_task);
}

/*!
 * Waits until the thread pool has executed all the tasks in the queue.
 *
 * \param this_ptr - A pointer to the task parser.
 */
void task_parser_wait(task_parser_t* this_ptr)
{
    thread_pool_wait(this_ptr->thread_pool);
}

/*!
 * Destroys and deallocates a task parser handle.
 *
 * \param this_ptr - A pointer to the task parser handle.
 */
void task_parser_destroy(task_parser_t *task_parser)
{
    thread_pool_destroy(task_parser->thread_pool);
    free(task_parser);
}

/*!
 * This is callback from the thread pool which sends the next task that should
 * be executed as an argument.
 *
 * \param task - A pointer to \c task_parser_task_t struct.
 *
 * \return \c TASK_PARSER_EXEC_SUCCESS if the task executed successfully.
 */
static int task_parser_exec(void *task)
{
    task_parser_task_t *task_parser_task = (task_parser_task_t*) task;
    task_parser_task->task(task_parser_task);
    free(task_parser_task);
    return TASK_PARSER_EXEC_SUCCESS;
}

/*!
 * This is a task which reads a config file.
 *
 * \param arg - A pointer to the arguments that the task needs.
 */
static void task_parser_read_file(void *arg)
{
    task_parser_task_t *task_parser_task = arg;

    config_parser_t *config = config_parser_open(task_parser_task->arg);
    config_parser_set_namespace(config, "default");

    while (!config_parser_is_eof(config)) {

        if (config_parser_read(config) == PARSER_OK) {
            printf("[%s] ",config_parser_get_namespace(config));
            printf("%s=",config_parser_get_command(config));
            while ((arg = config_parser_get_next_argument(config)) != NULL) {
                printf("%s ",arg);
            }
            printf("\n");
        }
    }
    config_parser_close(config);
}
