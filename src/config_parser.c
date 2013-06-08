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

#include "config_parser.h"

#include <stdlib.h>
#include <string.h>


typedef enum {
    PARSER_STATE_NEW_LINE,
    PARSER_STATE_COMMENT,
    PARSER_STATE_COMMAND,
    PARSER_STATE_POST_COMMAND,
    PARSER_STATE_PRE_ARGUMENT,
    PARSER_STATE_ARGUMENT,
    PARSER_STATE_ARGUMENT_TEXT,
    PARSER_STATE_POST_ARGUMENT,
    PARSER_STATE_POST_ARGUMENT_NEW_LINE,
    PARSER_STATE_PRE_NAMESPACE,
    PARSER_STATE_NAMESPACE,
    PARSER_STATE_POST_NAMESPACE,
    PARSER_STATE_ERROR
} parser_state_t;


int config_parser_read_file(const char* filename, config_handler_t *handler)
{
    FILE* file = fopen(filename, "r");
    parser_state_t state = PARSER_STATE_NEW_LINE;

    char parser_buffer[MAX_LENGTH];
    char command_buffer[MAX_COMMAND];

    char* error_msg;
    char* buffer_ptr;
    char* char_ptr;
    char* last_char_ptr;

    bool exit = false;

    int result = PARSER_OK;
    int line = 1;
    int size;

    if (file == NULL) {
        return PARSER_MISSING_FILE;
    }

    handler->func_start_config(handler->handler);

    while (!exit) {

        if (!feof(file) && !ferror(file)) {
            size = fread(parser_buffer, 1, MAX_LENGTH, file);
        } else {
            /* Fake a new line if there is no at the end. This simplifies
               the parser since there will always be an extra newline to end
               the file. */
            parser_buffer[0] = '\n';
            size = 1;
            exit = true;
        }

        /* Start at the beginning of the parser buffer. */
        char_ptr = parser_buffer;
        /* Calculate the last character. */
        last_char_ptr = char_ptr + size;

        while (char_ptr < last_char_ptr) {
            fprintf(stderr, "STATE: %d %c\n", state, *char_ptr);

            switch(state) {

                case PARSER_STATE_NEW_LINE:
                    switch (*char_ptr) {
                        case '[':
                            state = PARSER_STATE_NAMESPACE;
                            buffer_ptr = command_buffer;
                            break;

                        case '=':
                            state = PARSER_STATE_ERROR;
                            error_msg = "Missing command.";
                            break;

                        case '\n':
                        case ' ':
                        case '\t':
                            /* Ignore whitespace. */
                            break;

                        case '#':
                            state = PARSER_STATE_COMMENT;
                            break;

                        default:
                            state = PARSER_STATE_COMMAND;
                            buffer_ptr = command_buffer;
                            char_ptr--;
                            break;
                    }
                    break;

                case PARSER_STATE_NAMESPACE:
                    switch (*char_ptr) {
                        case ']':
                            state = PARSER_STATE_POST_NAMESPACE;
                            *buffer_ptr = '\0';
                            break;

                        case '\n':
                        case ' ':
                        case '\t':
                        case '#':
                            state = PARSER_STATE_ERROR;
                            error_msg = "Error in namespace.";
                            char_ptr--;
                            break;

                        default:
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
                            break;
                    }
                    break;

                case PARSER_STATE_POST_NAMESPACE:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            handler->func_namespace(handler->handler,
                                                    command_buffer);
                            break;

                        case ' ':
                        case '\t':
                            /* Ignore whitespace. */
                            break;

                        case '#':
                            state = PARSER_STATE_COMMENT;
                            handler->func_namespace(handler->handler,
                                                    command_buffer);
                            break;

                        default:
                            state = PARSER_STATE_ERROR;
                            error_msg = "Error in namespace.";
                            break;
                    }
                    break;

                case PARSER_STATE_COMMAND:
                    switch (*char_ptr) {
                        case ' ':
                        case '\t':
                            state = PARSER_STATE_POST_COMMAND;
                            *buffer_ptr = '\0';
                            break;

                        case '=':
                            state = PARSER_STATE_PRE_ARGUMENT;
                            *buffer_ptr = '\0';
                            handler->func_command(handler->handler,
                                                  command_buffer);
                            break;

                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            *buffer_ptr = '\0';
                            handler->func_command(handler->handler,
                                                  command_buffer);
                            break;

                        case '#':
                            state = PARSER_STATE_COMMENT;
                            handler->func_command(handler->handler,
                                                  command_buffer);
                            break;

                        default:
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
                            break;
                    }
                    break;

                case PARSER_STATE_POST_COMMAND:
                    switch (*char_ptr) {
                        case '=':
                            state = PARSER_STATE_PRE_ARGUMENT;
                            handler->func_command(handler->handler,
                                                  command_buffer);
                            break;

                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            handler->func_command(handler->handler,
                                                  command_buffer);
                            break;

                        case '#':
                            state = PARSER_STATE_COMMENT;
                            handler->func_command(handler->handler,
                                                  command_buffer);
                            break;

                        case ' ':
                        case '\t':
                            /* ignore white space. */
                            break;

                        default:
                            state = PARSER_STATE_ERROR;
                            error_msg = "Whitespace not supported in command.";
                            break;
                    }
                    break;

                case PARSER_STATE_PRE_ARGUMENT:
                    switch (*char_ptr) {
                        case ' ':
                        case '\t':
                            /* Ignore white space. */
                            break;

                        case '\n':
                            state = PARSER_STATE_ERROR;
                            error_msg = "Missing argument.";
                            char_ptr--;
                            break;

                        case '"':
                            state = PARSER_STATE_ARGUMENT_TEXT;
                            buffer_ptr = command_buffer;
                            break;

                        default:
                            state = PARSER_STATE_ARGUMENT;
                            buffer_ptr = command_buffer;
                            char_ptr--;
                            break;
                    }
                    break;

                case PARSER_STATE_ARGUMENT:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            *buffer_ptr = '\0';
                            handler->func_argument(handler->handler,
                                                   command_buffer);
                            break;

                        case '\\':
                            state = PARSER_STATE_POST_ARGUMENT_NEW_LINE;
                            break;

                        case ' ':
                        case '\t':
                            state = PARSER_STATE_POST_ARGUMENT;
                            *buffer_ptr = '\0';
                            handler->func_argument(handler->handler,
                                                   command_buffer);
                            break;

                        case '"':
                            state = PARSER_STATE_ERROR;
                            error_msg = "Missing \" character.";
                            result = PARSER_ERROR;
                            break;

                        case '#':
                            state = PARSER_STATE_COMMENT;
                            *buffer_ptr = '\0';
                            handler->func_argument(handler->handler,
                                                   command_buffer);
                            break;

                        default:
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
                            break;
                    }
                    break;

                case PARSER_STATE_ARGUMENT_TEXT:
                    switch (*char_ptr) {
                        case '"':
                            state = PARSER_STATE_POST_ARGUMENT;
                            *buffer_ptr = '\0';
                            handler->func_argument(handler->handler,
                                                   command_buffer);
                            buffer_ptr = command_buffer;
                            break;

                        default:
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
                            break;
                    }
                    break;

                case PARSER_STATE_POST_ARGUMENT:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            break;

                        case ' ':
                        case '\t':
                            /* ignore white space. */
                            break;

                        case '\\':
                            state = PARSER_STATE_POST_ARGUMENT_NEW_LINE;
                            buffer_ptr = command_buffer;
                            break;

                        case '#':
                            state = PARSER_STATE_COMMENT;
                            break;

                        case '"':
                            state = PARSER_STATE_ARGUMENT_TEXT;
                            buffer_ptr = command_buffer;
                            break;

                        default:
                            state = PARSER_STATE_ARGUMENT;
                            buffer_ptr = command_buffer;
                            char_ptr--;
                            break;
                    }
                    break;

                case PARSER_STATE_POST_ARGUMENT_NEW_LINE:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_PRE_ARGUMENT;
                            if (buffer_ptr != command_buffer) {
                                *buffer_ptr = '\0';
                                handler->func_argument(handler->handler,
                                                       command_buffer);
                            }
                            break;

                        case ' ':
                        case '\t':
                            /* ignore white space. */
                            break;

                        default:
                            state = PARSER_STATE_ERROR;
                            error_msg = "Missing new line character.";
                            result = PARSER_ERROR;
                            break;
                    }
                    break;

                case PARSER_STATE_ERROR:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            handler->func_error(handler->handler, filename,
                                                line, error_msg);
                            break;

                        default:
                            /* Do nothing. */
                            break;
                    }
                    result = PARSER_ERROR;
                    break;

                case PARSER_STATE_COMMENT:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            break;

                        default:
                            /* Ignore everything since this is a comment. */
                            break;
                    }
                    break;

                default:
                    /* This should never happened, but if it does, fallback
                       to a new line. */
                    state = PARSER_STATE_NEW_LINE;
                    handler->func_error(handler->handler, filename,
                                        line, "Unknown state.");
                    result = PARSER_ERROR;
                    break;
            }

            if (*char_ptr == '\n') {
                line = line + 1;
            }

            /* Get the next character. */
            char_ptr++;
        }
    }

    handler->func_end_config(handler->handler);

    fclose(file);
    return result;
}
