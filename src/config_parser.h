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

#ifndef _SPEEDY_CONFIG_PARSER_H_
#define _SPEEDY_CONFIG_PARSER_H_

#include <stdio.h>
#include <stdbool.h>

#define MAX_LENGTH 1024u
#define MAX_ARGUMENT 512u
#define MAX_COMMAND 64u

#define PARSER_OK 0
#define PARSER_NO_DATA -1
#define PARSER_ERROR -2

typedef struct config_parser_t {
    char buffer[MAX_LENGTH];
    char command[MAX_COMMAND];
    char name_space[MAX_COMMAND];
    char argument[MAX_ARGUMENT];
    char *error_msg;
    char *buffer_pos_ptr;
    FILE* file;
    size_t buffer_pos;
    size_t bytes_read;
    unsigned int line;
    unsigned int mode;
    unsigned int argument_size;
    char *next_argument_pos_ptr;
    bool eof;
} config_parser_t;

config_parser_t *config_parser_open(const char* filename);
void config_parser_close(config_parser_t *config);

bool config_parser_is_eof(config_parser_t *config);

int config_parser_read(config_parser_t *config);

void config_parser_set_namespace(config_parser_t *config, const char *str);
const char* config_parser_get_namespace(config_parser_t *config);

const char* config_parser_get_command(config_parser_t *config);

const char *config_parser_get_error_msg(config_parser_t *config);

const char* config_parser_get_next_argument(config_parser_t *config);
unsigned int config_parser_get_argument_size(config_parser_t *config);
char** config_parser_create_arguments(config_parser_t *config);
void config_parser_destroy_arguments(char* arguments[]);

#endif /* _SPEEDY_CONFIG_PARSER_H_ */
