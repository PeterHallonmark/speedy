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

#include "queue.h"
#include "config_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct parser_command_t {
    char *command;
    char *argument;
    char *error_msg;
    unsigned int line;
} parser_command_t;


static void config_parser_readfile(config_parser_t *config);


config_parser_t *config_parser_open(char* filename)
{
    config_parser_t *config = malloc(sizeof(config_parser_t));

    if ((config != NULL) &&
        ((config->file = fopen(filename, "r")) == NULL)) {

        free(config);
        config = NULL;

    } else {
        config->command[0] = '\0';
        config->argument[0] = '\0';
        config->buffer_pos = 0;
        config->bytes_read = 0;
        config->line = 0u;
        config->eof = false;
    }
    return config;
}

void config_parser_read(config_parser_t *config)
{
    char *command_pos_ptr;
    char *argument_pos_ptr;

    config->mode = NEW_LINE;

    while ((config->mode != EXIT) && (config->mode != ERROR)) {

        config_parser_readfile(config);

        while ((config->buffer_pos < config->bytes_read) &&
               (config->mode != EXIT) && (config->mode != ERROR)) {

            switch(config->mode) {
                case NEW_LINE:
                    command_pos_ptr = config->command;
                    argument_pos_ptr = config->argument;
                    *command_pos_ptr = '\0';
                    *argument_pos_ptr = '\0';
                    config->line++;
                    config->mode = PRE_COMMAND;
                    break;

                case PRE_COMMAND:
                    switch(*config->buffer_pos_ptr) {
                        case ' ':
                        case '\t':
                            config->buffer_pos_ptr++;
                            config->buffer_pos++;
                            break;

                        default:
                            config->mode = COMMAND;
                            break;
                    }
                    break;

                case COMMAND:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;

                        case '#':
                            config->mode = COMMENT;
                            break;

                        case '=':
                            if (command_pos_ptr != config->command) {
                                config->mode = PRE_ARGUMENT;
                            } else {
                                //command_data->error_msg = "Missing command.";
                                config->mode = PRE_ERROR;
                            }
                            break;

                        case ' ':
                        case '\t':
                            config->mode = SPACE;
                            break;

                        default:
                            if (command_pos_ptr <
                                (config->command + MAX_COMMAND)) {

                                *command_pos_ptr = *config->buffer_pos_ptr;
                                command_pos_ptr++;
                            } else {
                                //command_data->error_msg = "Command to long.";
                                config->mode = PRE_ERROR;
                            }
                            break;
                    }
                    config->buffer_pos_ptr++;
                    config->buffer_pos++;
                    break;

                case SPACE:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;

                        case '=':
                            if (command_pos_ptr != config->command) {
                                config->mode = PRE_ARGUMENT;
                            } else {
                                //command_data->error_msg = "Missing command.";
                                config->mode = PRE_ERROR;
                            }
                            break;

                        case '#':
                            config->mode = COMMENT;
                            break;

                        case ' ':
                        case '\t':
                            /* Do nothing. */
                            break;

                        default:
                            //command_data->error_msg = "Space is not supported in a command.";
                            config->mode = PRE_ERROR;
                            break;
                    }
                    config->buffer_pos_ptr++;
                    config->buffer_pos++;
                    break;

                case PRE_ARGUMENT:
                    switch(*config->buffer_pos_ptr) {
                        case ' ':
                        case '\t':
                            config->buffer_pos_ptr++;
                            config->buffer_pos++;
                            break;

                        default:
                            config->mode = ARGUMENT;
                            break;
                    }
                    break;

                case ARGUMENT:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;

                        case '#':
                            config->mode = COMMENT;
                            break;

                        default:
                            if (argument_pos_ptr <
                                (config->argument + MAX_ARGUMENT)) {

                                *argument_pos_ptr = *config->buffer_pos_ptr;
                                argument_pos_ptr++;
                            } else {
                                //command_data->error_msg = "Argument to long.";
                                config->mode = PRE_ERROR;
                            }
                            break;
                    }
                    config->buffer_pos_ptr++;
                    config->buffer_pos++;
                    break;

                case COMMENT:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;

                        default:
                            /* Do nothing. */
                            break;
                    }
                    config->buffer_pos_ptr++;
                    config->buffer_pos++;
                    break;

                case ADD_COMMAND:
                    if (command_pos_ptr != config->command) {
                        config->mode = EXIT;
                        *command_pos_ptr = '\0';
                        *argument_pos_ptr = '\0';
                        printf("%s=%s\n",config->command, config->argument);
                    } else {
                        config->mode = NEW_LINE;
                    }
                    break;

                case PRE_ERROR:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->argument[0] = '\0';
                            config->command[0] = '\0';
                            config->mode = ERROR;
                            break;

                        default:
                            /* Do nothing. */
                            break;
                    }
                    config->buffer_pos_ptr++;
                    config->buffer_pos++;
                    break;

                default:
                    config->buffer_pos_ptr++;
                    config->buffer_pos++;
                    break;

            }
        }
    }
}

bool config_parser_is_eof(config_parser_t *config)
{
    if (config != NULL) {
        return config->eof;
    }
    return true;
}


/*void config_parser_print(config_parser_t *config_data)
{
    parser_command_t *command_data;

    while ((command_data = queue_pop(config_data->queue)) != NULL) {
    printf("command: %s arg: %s error %s\n",command_data->command, command_data->argument, command_data->error_msg);    
    }
}*/

static void config_parser_readfile(config_parser_t *config)
{
    /* Check if the buffer is empty. */
    if (config->buffer_pos == config->bytes_read) {

        if ((feof(config->file) == 0) && (ferror(config->file) == 0)) {
            config->bytes_read = fread(config->buffer, 1, MAX_LENGTH,
                                       config->file);
            config->buffer_pos_ptr = config->buffer;
            config->buffer_pos = 0u;
        } else {

            config->mode = EXIT;
            config->eof = true;
        }
    }
}
