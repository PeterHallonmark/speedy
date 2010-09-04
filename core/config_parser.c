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

#define MAX_LENGTH 1024u
#define MAX_ARGUMENT 1024u
#define MAX_COMMAND 128u

typedef struct parser_command_t {
    char *command;
    char *argument;
    char *error_msg;
    unsigned int line;
} parser_command_t;

typedef enum {
    NEW_LINE,
    SPACE,
    COMMENT,
    COMMAND,
    ARGUMENT,
    ADD_COMMAND
} parser_mode_t;

config_parser_t *config_parser_readfile(char* filename)
{
    config_parser_t *config_parser = malloc(sizeof(config_parser_t));
    char buffer[MAX_LENGTH];
    char command[MAX_COMMAND];
    char argument[MAX_ARGUMENT];
    size_t bytes_read;
    FILE* file;
    char *buffer_pos;
    char *command_pos;
    char *argument_pos;
    unsigned int pos;
    unsigned int line = 0u;
    parser_mode_t mode = NEW_LINE;
    parser_command_t *command_data = NULL;

    if (config_parser != NULL) {
        if ((config_parser->queue = queue_create()) == NULL) {
            free(config_parser);
            return NULL;
        }

        file = fopen(filename, "r");

        while ((feof(file) == 0) && (ferror(file) == 0)) {
            bytes_read = fread(buffer, 1, MAX_LENGTH, file);
            buffer_pos = buffer;
            pos = 0u;

            while (pos < bytes_read) {

                switch(mode) {
                    case NEW_LINE:
                        command_pos = command;
                        argument_pos = argument;
                        *command_pos = '\0';
                        *argument_pos = '\0';
                        line++;
                        mode = COMMAND;
                        command_data = malloc(sizeof(parser_command_t));
                        command_data->error_msg = NULL;
                        break;
                    
                    case COMMAND:
                        switch(*buffer_pos) {
                            case '\n':
                                mode = ADD_COMMAND;
                                *command_pos = '\0';
                                break;

                            case '#':
                                mode = COMMENT;
                                *command_pos = '\0';
                                break;

                            case '=':
                                if (command_pos != command) {
                                    mode = ARGUMENT;
                                    *command_pos = '\0';
                                } else {
                                    command_data->error_msg = "Missing command.";
                                }
                                break;

                            case ' ':
                            case '\t':
                                if (command_pos != command) {
                                    mode = SPACE;
                                    *command_pos = '\0';
                                }
                                break;

                            default:
                                if (command_pos < (command + MAX_COMMAND)) {
                                    *command_pos = *buffer_pos;
                                    command_pos++;
                                } else {
                                    command_data->error_msg = "Command to long.";
                                }
                                break;
                        }
                        buffer_pos++;
                        pos++;                        
                        break;

                    case SPACE:
                        switch(*buffer_pos) {
                            case '\n':
                                mode = ADD_COMMAND;
                                break;

                            case '=':
                                if (command_pos != command) {
                                    mode = ARGUMENT;
                                } else {
                                    command_data->error_msg = "Missing command.";
                                }
                                break;

                            case '#':
                                mode = COMMENT;
                                break;

                            case ' ':
                            case '\t':
                                /* Do nothing. */
                                break;

                            default:
                                command_data->error_msg = "Space is not supported in a command.";
                                break;
                        }
                        buffer_pos++;
                        pos++;                        
                        break;

                    case ARGUMENT:
                        switch(*buffer_pos) {
                            case '\n':
                                mode = ADD_COMMAND;
                                break;

                            case '#':
                                mode = COMMENT;
                                break;

                            default:
                                if (argument_pos < (argument + MAX_ARGUMENT)) {
                                    *argument_pos = *buffer_pos;
                                    argument_pos++;
                                } else {
                                    command_data->error_msg = "Argument to long.";
                                }
                                break;
                        }
                        buffer_pos++;
                        pos++;
                        break;

                    case COMMENT:
                        switch(*buffer_pos) {
                            case '\n':
                                mode = ADD_COMMAND;
                                break;

                            default:
                                /* Do nothing. */
                                break;
                        }
                        buffer_pos++;
                        pos++;
                        break;

                    case ADD_COMMAND:
                        if ((command_pos != command) || (command_data->error_msg != NULL)) {
                            command_data->line = line;
                            command_data->command = strdup(command);
                            command_data->argument = strdup(argument);
                            queue_push(config_parser->queue, command_data);
                            command_data = NULL;
                            mode = NEW_LINE;
                        } else {
                            mode = COMMAND;
                            line++;
                        }
                        
                        break;
                }        
            }
        }
    }
    return config_parser;
}


void config_parser_print(config_parser_t *config_data)
{
    parser_command_t *command_data;

    while ((command_data = queue_pop(config_data->queue)) != NULL) {
    printf("command: %s arg: %s error %s\n",command_data->command, command_data->argument, command_data->error_msg);    
    }
}
