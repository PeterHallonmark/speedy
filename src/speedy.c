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

#include "core_type.h"
#include "hash_lookup.h"
#include "queue.h"
#include "config_parser.h"
//#include "task_handler.h"
//#include "task_parser.h"

#include <stdio.h>
#include <stdlib.h>


#define NOT_USED(var) (void) var


void start_callback(void *handler)
{
    NOT_USED(handler);
}

void end_callback(void *handler)
{
    NOT_USED(handler);
}

void namespace_callback(void *handler, const char *name)
{
    NOT_USED(handler);
    printf("Namespace: %s\n",name);
}

void command_callback(void *handler, const char *command)
{
    NOT_USED(handler);
    printf("Command: %s\n",command);
}

void argument_callback(void *handler, const char *argument)
{
    NOT_USED(handler);
    printf("Argument: %s\n",argument);
}

void error_callback(void *handler, const char* filename, int line,
                    const char *error_msg)
{
    NOT_USED(handler);
    printf("Error: %s:%d: %s\n",filename, line, error_msg);
}


/*!
 * The main function for Speedy.
 *
 * \param argc - Number of parameters from the command line.
 * \param argv - Array of parameters from the command line.
 *
 * \return success
 */
int main(int argc, char *argv[])
{
    /*task_handler_t *task_handler = task_handler_create();
    task_parser_t *task_parser = task_parser_create(task_handler);
    */
    NOT_USED(argc);
    NOT_USED(argv);

    config_handler_t test;

    test.handler = NULL;
    test.func_start_config = &start_callback;
    test.func_end_config = &end_callback;
    test.func_namespace = &namespace_callback;
    test.func_command = &command_callback;
    test.func_argument = &argument_callback;
    test.func_error = &error_callback;

    config_parser_read_file("config/speedy.conf", &test);


    /* Read which tasks that need to be executed and all the dependency
       information from the configuration. */
    /*task_parser_read(task_parser, "config/speedy.conf");
    task_parser_wait(task_parser);
    */
    /* Read the dependency from the configuration. */
    //task_handler_calculate_dependency(task_handler);
    //task_handler_wait(task_handler);

    //task_parser_destroy(task_parser);
    //task_handler_destroy(task_handler);
    return 0;
}
