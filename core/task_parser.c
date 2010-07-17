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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH 1024u
#define MAX_COMMAND 128u

typedef enum {
    COMMENT,
    COMMAND,
    TEXT
} parser_mode_t;


service_t * task_parser_readfile(const char* filename)
{
    service_t *service = malloc(sizeof(service_t));
    char buffer[MAX_LENGTH];
    char command[MAX_COMMAND];
    size_t bytes_read;
    FILE* file;
    char *buffer_pos;
    char *command_pos = command;
    unsigned int pos;
    unsigned int line = 1u;
    parser_mode_t mode = COMMAND;

    service->name = NULL;
    service->dependency = NULL;
    service->provides = NULL;
    service->action = NULL;

    file = fopen(filename, "r");
    
    if (file == NULL) {
        free(service);
        service = NULL;
    }

    if (service != NULL) {
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
                                    printf("%s:%d: Parse error: %s\n",filename, line, command);
                                }
                                line++;
                                command_pos = command;
                                break;

                            case '#':
                                mode = COMMENT;
                                *command_pos = '\0';
                                if (command_pos != command) {
                                    printf("%s:%d: Parse error: %s\n",filename, line, command);
                                }
                                command_pos = command;
                                break;

                            case '=':
                                mode = TEXT;
                                *command_pos = '\0';
                                printf("command: %s\n",command);
                                command_pos = command;
                                break;

                            case ' ':
                            case '\t':
                                /* Do nothing. */
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

                    case TEXT:
                        switch(*buffer_pos) {
                            case '\n':
                                line++;
                                mode = COMMAND;
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

    return service;
}

void task_parser_destroy_service(service_t *service)
{
    free(service);
}
