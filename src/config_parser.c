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

#include "config_parser.h"

#include <stdlib.h>
#include <string.h>

typedef enum {
    NEW_LINE,
    SPACE,
    COMMENT,
    PRE_COMMAND,
    COMMAND,
    ADD_COMMAND,
    PRE_ARGUMENT,
    ARGUMENT,
    ARGUMENT_TEXT,
    ADD_ARGUMENT,
    NAMESPACE,
    POST_NAMESPACE,
    EXIT,
    PRE_ERROR,
    ERROR
} parser_mode_t;

static void config_parser_readfile(config_parser_t *config);

config_parser_t *config_parser_open(const char* filename)
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

void config_parser_close(config_parser_t *config)
{
    if (config != NULL) {
        fclose(config->file);
        free(config);
        config = NULL;
    }
}

/*!
 * Parse a file and exit when a command has been found.
 *
 * The implementation is made so it caches part of a file and checks the cached
 * data for commands, arguments and namespaces. This function will not return
 * until it has find a command and any eventually arguments for it.
 *
 * \param config - A pointer to its own config parser handle.
 *
 * \note This implementation has not been devided into smaller pieces mainly
 *       because it is supposed to be run often and mainly to limit any
 *       performance penalties that otherwise can happen because of 
 *       sub functions.
 *
 * \return \c PARSER_OK if everything went well, otherwise return an error code.
 */
int config_parser_read(config_parser_t *config)
{
    char *command_pos_ptr = NULL;
    char *argument_pos_ptr = NULL;
    char *namespace_pos_ptr = NULL;
    bool argument_exist = false;
    int status = PARSER_NO_DATA;

    config->mode = NEW_LINE;
    config->argument_size = 0u;
    config->next_argument_pos_ptr = config->argument;

    while ((config->mode != EXIT) && (config->mode != ERROR)) {

        config_parser_readfile(config);

        while (((config->buffer_pos < config->bytes_read) &&
               (config->mode != EXIT) && (config->mode != ERROR)) ||
               (config->mode == ADD_COMMAND)) {

            switch(config->mode) {
                case NEW_LINE:
                    command_pos_ptr = config->command;
                    argument_pos_ptr = config->argument;
                    namespace_pos_ptr = config->name_space;
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

                        case '[':
                            config->mode = NAMESPACE;
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

                        case '"':
                            config->mode = ARGUMENT_TEXT;
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
                            if (argument_exist) {
                                argument_exist = false;
                                config->argument_size++;
                            }

                            config->mode = ADD_COMMAND;
                            break;

                        case '#':
                            config->mode = COMMENT;
                            break;

                        case '"':
                            config->mode = PRE_ERROR;
                            break;

                        case ' ':
                        case '\t':
                            config->mode = ADD_ARGUMENT;
                            break;

                        default:
                            if (argument_pos_ptr <
                                (config->argument + MAX_ARGUMENT)) {

                                argument_exist = true;
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

                case ARGUMENT_TEXT:
                        switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = PRE_ERROR;
                            break;

                        case '"':
                            config->mode = ADD_ARGUMENT;
                            break;

                        default:
                            if (argument_pos_ptr <
                                (config->argument + MAX_ARGUMENT)) {

                                argument_exist = true;
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

                case NAMESPACE:
                    switch(*config->buffer_pos_ptr) {
                        case ']':
                            if (namespace_pos_ptr <
                                (config->name_space + MAX_COMMAND)) {

                                *namespace_pos_ptr = '\0';
                                namespace_pos_ptr++;
                                config->mode = POST_NAMESPACE;
                            } else {
                                //command_data->error_msg = "Command to long.";
                                config->mode = PRE_ERROR;
                            }
                            break;

                        default:
                            if (namespace_pos_ptr <
                                (config->name_space + MAX_COMMAND)) {

                                *namespace_pos_ptr = *config->buffer_pos_ptr;
                                namespace_pos_ptr++;
                            } else {
                                //command_data->error_msg = "Command to long.";
                                config->mode = PRE_ERROR;
                            }
                            break;
                    }
                    config->buffer_pos_ptr++;
                    config->buffer_pos++;
                    break;

                case POST_NAMESPACE:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;

                        case ' ':
                        case '\t':
                            /* Do nothing. */
                            break;

                        case '#':
                            config->mode = COMMENT;
                            break;

                        default:
                            config->mode = PRE_ERROR;
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

                case ADD_ARGUMENT:
                    if (argument_pos_ptr < (config->argument + MAX_ARGUMENT)) {
                        *argument_pos_ptr = '\0';
                        argument_pos_ptr++;

                        if (argument_exist) {
                            argument_exist = false;
                            config->argument_size++;
                        }
                        config->mode = PRE_ARGUMENT;
                    } else {
                        //command_data->error_msg = "Argument to long.";
                        config->mode = PRE_ERROR;
                    }
                    config->mode = PRE_ARGUMENT;
                    break;

                case ADD_COMMAND:
                    if ((command_pos_ptr == NULL) ||
                        (argument_pos_ptr == NULL)) {
                        config->mode = EXIT;
                    } else if (command_pos_ptr != config->command) {
                        config->mode = EXIT;
                        *command_pos_ptr = '\0';
                        *argument_pos_ptr = '\0';
                        status = PARSER_OK;
                    } else {
                        config->mode = NEW_LINE;
                    }
                    break;

                case PRE_ERROR:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->argument[0] = '\0';
                            config->command[0] = '\0';
                            config->argument_size = 0u;
                            config->mode = ERROR;
                            status = PARSER_ERROR;
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
    return status;
}

bool config_parser_is_eof(config_parser_t *config)
{
    if (config != NULL) {
        return config->eof;
    }
    return true;
}

void config_parser_set_namespace(config_parser_t *config, const char *str)
{
    strcpy(config->name_space, str);
}

const char* config_parser_get_namespace(config_parser_t *config)
{
    return config->name_space;
}


const char* config_parser_get_command(config_parser_t *config)
{
    return config->command;
}

const char* config_parser_get_next_argument(config_parser_t *config)
{
    char *argument = NULL;

    if (config->argument_size > 0) {
        config->argument_size--;

        argument = config->next_argument_pos_ptr;
        do {
            config->next_argument_pos_ptr++;
        } while (*(config->next_argument_pos_ptr) != '\0');
        config->next_argument_pos_ptr++;
    }
    return argument;
}

unsigned int config_parser_get_argument_size(config_parser_t *config)
{
    return config->argument_size + 1;
}

char** config_parser_create_arguments(config_parser_t *config)
{
    char **arguments = malloc(sizeof(char*) * (config->argument_size + 1));
    unsigned int i;

    if (arguments != NULL) {
        arguments[config->argument_size] = NULL;
        
        for (i = 0u; i < config->argument_size; i++) {
            arguments[i] = strdup(config_parser_get_next_argument(config));
        }
    }
    return arguments;
}

void config_parser_destroy_arguments(char* arguments[])
{
    unsigned int i = 0u;

    if (arguments != NULL) {
        while(arguments[i] != NULL) {
            free(arguments[i]);
            i++;
        }
        free(arguments);
    }
}

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
            if (config->mode == PRE_ERROR) {
                config->mode = ERROR;
            } else {
                config->mode = ADD_COMMAND;
            }
            config->eof = true;
        }
    }
}
