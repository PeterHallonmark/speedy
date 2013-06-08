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

#ifndef _SPEEDY_CONFIG_PARSER_H_
#define _SPEEDY_CONFIG_PARSER_H_

#include <stdio.h>
#include <stdbool.h>

#define MAX_LENGTH 1024u
#define MAX_ARGUMENT 512u
#define MAX_COMMAND 64u

#define PARSER_OK 0
#define PARSER_ERROR -2
#define PARSER_MISSING_FILE -3


typedef struct config_handler_t {
    void *handler;
    void (*func_start_config)(void *handler);
    void (*func_end_config)(void *handler);
    void (*func_namespace)(void *handler, const char *name);
    void (*func_command)(void *handler, const char *command);
    void (*func_argument)(void *handler, const char *argument);
    void (*func_error)(void *handler, const char* filename, int line,
                       const char *error_msg);
} config_handler_t;

int config_parser_read_file(const char* filename, config_handler_t *handler);


#endif /* _SPEEDY_CONFIG_PARSER_H_ */
