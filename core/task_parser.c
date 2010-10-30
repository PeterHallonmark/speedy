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
#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*! Successful return code for thread pool callback function. */
#define TASK_PARSER_EXEC_SUCCESS 0

typedef enum namespace_t {
    NAMESPACE_OPTIONS,
    NAMESPACE_CONFIG
} namespace_t;

typedef enum config_options_t {
    CONFIG_OPTIONS_DEPENDENCY,
    CONFIG_OPTIONS_PATH,
    CONFIG_OPTIONS_UNKOWN
} config_options_t;

typedef enum task_options_t {
    TASK_OPTIONS_NAME,
    TASK_OPTIONS_PROVIDES,
    TASK_OPTIONS_DEPENDENCY,
    TASK_OPTIONS_UNKOWN
} task_options_t;

/*!
 * A simpler structure for tasks which doesn't have dependencies.
 */
typedef struct task_parser_config_t {
    /*!< A pointer to the task parser handle. */
    task_parser_t *task_parser;
    /*! A pointer to the functional call for the task. */
    void (*task)(void *argument);
    /*! Filename for the configuration file. */
    void *filename;
    /*! Handle for the config file. */
    config_parser_t *file;
    /*! Default namespace for the configuration file. */
    const char *default_namespace;
    /*! Current namespace for the configuration task. */
    const char *current_namespace;
    /*! Extracted value for the current namespace. */
    namespace_t current_namespace_value;

    service_t *current_task;

    queue_t paths;

    queue_t tasks;
} task_parser_config_t;

static int task_parser_exec(void *task);

static void task_parser_read_file_exec(void *argument);
static void task_parser_read_file_init(task_parser_config_t *config);
static void task_parser_read_file_deinit(task_parser_config_t *config);

static void task_parser_add_task(task_parser_config_t *config);
static void task_parser_create_task(task_parser_config_t *config);

static void task_parser_handle_options(task_parser_config_t *config);
static void task_parser_handle_task(task_parser_config_t *config);

static namespace_t task_parser_get_namespace_value(const char *str_namespace);
static config_options_t task_parser_get_config_options(const char* str_command);
static task_options_t task_parser_get_task_options(const char* str_command);

/*!
 * Creates a task parser handle.
 *
 * \param this_ptr - A pointer to the task handler where all the tasks should
 *                   be registered.
 *
 * \return A pointer to the task parser handle if it was successfully created,
 *         \c NULL otherwise.
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
    task_parser_config_t *config = malloc(sizeof(task_parser_config_t));

    task_parser_read_file_init(config);
    config->task_parser = this_ptr;
    config->filename = (void*) filename;
    thread_pool_add_task(this_ptr->thread_pool, config);
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
    task_parser_config_t *config = (task_parser_config_t*) task;
    config->task(config);
    free(config);
    return TASK_PARSER_EXEC_SUCCESS;
}

/*!
 * This is a task which reads a config file.
 *
 * \param arg - A pointer to the arguments that the task needs.
 */
static void task_parser_read_file_exec(void *arg)
{
    task_parser_config_t *config = arg;

    config->file = config_parser_open(config->filename);
    config_parser_set_namespace(config->file, config->default_namespace);

    while (!config_parser_is_eof(config->file) &&
           (config_parser_read(config->file) == PARSER_OK)) {

        config->current_namespace = config_parser_get_namespace(config->file);
        config->current_namespace_value = task_parser_get_namespace_value(
                                              config->current_namespace);

        switch (config->current_namespace_value) {
            case NAMESPACE_OPTIONS:
                task_parser_handle_options(config);
                break;

            case NAMESPACE_CONFIG:
                task_parser_handle_task(config);
                break;

            default:
                /* Do nothing. */
                break;
        }
    }

    config_parser_close(config->file);
    task_parser_read_file_deinit(config);
}

static void task_parser_read_file_init(task_parser_config_t *config)
{
    config->task = task_parser_read_file_exec;
    config->default_namespace = "default";

    queue_init(&config->tasks);
    queue_init(&config->paths);

    task_parser_create_task(config);
}

static void task_parser_read_file_deinit(task_parser_config_t *config)
{
    char* path;
    char* task;

    if (config->current_task->name != NULL) {
        task_parser_add_task(config);
    }
    free(config->current_task);

    /* Free all the option paths. */
    while((path = queue_pop(&config->paths)) != NULL) {
        free(path);
    }
    queue_deinit(&config->paths);

    /* Free all the remaining tasks and print an error since these were not
       possible to find in either the current file or in the paths. */
    while((task = queue_pop(&config->tasks)) != NULL) {
        free(task);
    }
    queue_deinit(&config->tasks);
}


static void task_parser_add_task(task_parser_config_t *config)
{
    free(config->current_task->name);
    free(config->current_task->dependency);
    free(config->current_task->provides);
    free(config->current_task->action);
    free(config->current_task);
    config->current_task = NULL;
}

static void task_parser_create_task(task_parser_config_t *config)
{
    /* Create a new task and initialize it. */
    config->current_task = malloc(sizeof(service_t));
    config->current_task->dependency = NULL;
    config->current_task->provides = NULL;
    config->current_task->action = NULL;
}

/*!
 * Handles any options which has been parsed from the configuration.
 *
 * \param config - Contains the local settings for the current
 *                 parser task.
 */
static void task_parser_handle_options(task_parser_config_t *config)
{
    config_options_t options;
    const char *command;
    const char *argument;

    command = config_parser_get_command(config->file);
    options = task_parser_get_config_options(command);

    switch (options) {
        case CONFIG_OPTIONS_DEPENDENCY:
            argument = config_parser_get_next_argument(config->file);
            while (argument != NULL) {
                queue_push(&config->tasks, strdup(argument));                
                argument = config_parser_get_next_argument(config->file);
            }
            break;

        case CONFIG_OPTIONS_PATH:
            argument = config_parser_get_next_argument(config->file);
            while (argument != NULL) {
                queue_push(&config->paths, strdup(argument));
                argument = config_parser_get_next_argument(config->file);
            }
            break;

        case CONFIG_OPTIONS_UNKOWN:
        default:
            break;
    }
}

/*!
 * Handles a task which has been parsed from the configuration.
 *
 * \param config - Contains the local settings for the current
 *                 parser task.
 */
static void task_parser_handle_task(task_parser_config_t *config)
{
    task_options_t options;
    const char *command;
    const char *argument;

    if (config->current_task->name == NULL) {
        config->current_task->name = strdup(config->current_namespace);

    } else if (strcmp(config->current_namespace,
                      config->current_task->name) != 0) {

        /* Add the current task object. */
        task_parser_add_task(config);

        /* Create a new task object and name it according to the
           new namespace. */
        task_parser_create_task(config);
        config->current_task->name = strdup(config->current_namespace);
    }
    command = config_parser_get_command(config->file);
    options = task_parser_get_task_options(command);

    switch (options) {
        case TASK_OPTIONS_DEPENDENCY:

            break;

        case TASK_OPTIONS_PROVIDES:
            argument = config_parser_get_next_argument(config->file);
            printf("provides: %s\n", argument);

            if ((config->current_task->provides == NULL) &&
                (argument != NULL)) {

                config->current_task->provides = strdup(argument);
            } else {
                printf("some error...\n");
            }
            break;


        default:
            break;
    }
}

/*!
 * Gets the config namespace value from a string.
 * \note This is separated here for readability.
 *
 * \param str_namespace - A string which needs to be transformed into an
 *                        integer value.
 *
 * \return The string value represented as an integer value.
 */
static namespace_t task_parser_get_namespace_value(const char *str_namespace)
{
    if (strcmp(str_namespace, "options") == 0) {
        return NAMESPACE_OPTIONS;

    } else {
        return NAMESPACE_CONFIG;
    }
}

/*!
 * Gets the config option value from a string.
 * \note This is separated here for readability.
 *
 * \param str_command - A string which needs to be transformed into an
 *                      integer value.
 *
 * \return The string value represented as an integer value.
 */
static config_options_t task_parser_get_config_options(const char* command)
{
    if (strcmp(command, "dependency") == 0) {
        return CONFIG_OPTIONS_DEPENDENCY;

    } else if (strcmp(command, "path") == 0) {
        return CONFIG_OPTIONS_PATH;

    } else {
        return CONFIG_OPTIONS_UNKOWN;
    }
}

/*!
 * Gets the config option value from a string.
 * \note This is separated here for readability.
 *
 * \param str_command - A string which needs to be transformed into an
 *                      integer value.
 *
 * \return The string value represented as an integer value.
 */
static task_options_t task_parser_get_task_options(const char* command)
{
    if (strcmp(command, "dependency") == 0) {
        return TASK_OPTIONS_DEPENDENCY;

    } else if (strcmp(command, "provides") == 0) {
        return TASK_OPTIONS_PROVIDES;

    } else {
        return TASK_OPTIONS_UNKOWN;
    }
}
