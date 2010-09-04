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
#define MAX_COMMAND 128u

typedef struct parser_command_t {
    char *command;
    char *argument;
    unsigned int line;
} parser_command_t;

typedef enum {
    SPACE,
    COMMENT,
    COMMAND,
    TEXT
} parser_mode_t;


config_parser_t *config_parser_readfile(char* filename)
{
    config_parser_t *config_parser = malloc(sizeof(config_parser_t));
    char buffer[MAX_LENGTH];
    char command[MAX_COMMAND];
    size_t bytes_read;
    FILE* file;
    char *buffer_pos;
    char *command_pos = command;
    unsigned int pos;
    unsigned int line = 1u;
    parser_mode_t mode = COMMAND;
    parser_command_t command_data;

    if (config_parser != NULL) {
        /*if ((config_parser->queue = queue_create()) == NULL) {
            free(config_parser);
            return NULL;
        }*/

        file = fopen(filename, "r");

        while ((feof(file) == 0) && (ferror(file) == 0)) {
            bytes_read = fread(buffer, 1, MAX_LENGTH, file);
            buffer_pos = buffer;
            pos = 0u;

            while (pos < bytes_read) {

                switch(mode) {

                    case COMMAND:
                        switch(*buffer_pos) {
                            case '\n':
                                *command_pos = '\0';
                                if (command_pos != command) {
                                    command_data.line = line;
                                    command_data.command = strdup(command);
                                    command_data.argument = NULL;
                                }
                                line++;
                                command_pos = command;
                                break;

                            case '#':
                                mode = COMMENT;
                                *command_pos = '\0';
                                if (command_pos != command) {
                                    command_data.line = line;
                                    command_data.command = strdup(command);
                                    command_data.argument = NULL;
                                }
                                break;

                            case '=':
                                mode = TEXT;
                                *command_pos = '\0';
                                command_data.line = line;
                                command_data.command = strdup(command);
                                break;

                            case ' ':
                            case '\t':
                                mode = SPACE;
                                *command_pos = '\0';
                                if (command_pos != command) {
                                    command_data.line = line;
                                    command_data.command = strdup(command);
                                }
                                break;

                            default:
                                if (command_pos < (command + MAX_COMMAND)) {
                                    *command_pos = *buffer_pos;
                                    command_pos++;
                                } else {
                                    /* Report error. */
                                }
                                break;
                        }
                        break;

                    case SPACE:
                        switch(*buffer_pos) {
                            case '\n':
                                line++;
                                mode = COMMAND;
                                command_pos = command;
                                break;

                            case '=':

                                break;

                            case '#':
                                mode = COMMENT;
                                break;

                            case ' ':
                            case '\t':
                                /* Do nothing. */
                                break;

                            default:
                                /* error */
                                break;
                        }
                        break;

                    case TEXT:
                        switch(*buffer_pos) {
                            case '\n':
                                line++;
                                mode = COMMAND;
                                command_pos = command;
                                break;

                            case '#':
                                mode = COMMENT;
                                break;

                            default:
                                break;
                        }
                        break;

                    case COMMENT:
                        switch(*buffer_pos) {
                            case '\n':
                                line++;
                                mode = COMMAND;
                                command_pos = command;
                                break;

                            default:
                                /* Do nothing. */
                                break;
                        }
                        break;

                }
                buffer_pos++;
                pos++;
            }
        }
    }
    return NULL;
}
