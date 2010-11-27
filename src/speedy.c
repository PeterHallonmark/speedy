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
#include "hash_lookup.h"
#include "queue.h"
#include "task_handler.h"
#include "task_parser.h"

#include <stdio.h>
#include <stdlib.h>


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
    task_handler_t *task_handler = task_handler_create();
    task_parser_t *task_parser = task_parser_create(task_handler);

    /* Read which tasks that need to be executed and all the dependency
       information from the configuration. */
    task_parser_read(task_parser, "speedy.conf");
    task_parser_wait(task_parser);

    /* Read the dependency from the configuration. */
    task_handler_calculate_dependency(task_handler);
    task_handler_wait(task_handler);

    task_parser_destroy(task_parser);
    task_handler_destroy(task_handler);
    return 0;
}
