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


void config_parser_read_file(const char* filename, config_handler_t *handler)
{
    FILE* file = fopen(filename, "r");
    parser_state_t state = PARSER_STATE_NEW_LINE;

    char parser_buffer[MAX_LENGTH];
    char command_buffer[MAX_COMMAND];

    char* error_msg;
    char* buffer_ptr;
    char* char_ptr;

    bool exit = false;

    int line = 1;
    int size;
    int i;

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

        for (i = 0; i < size; i++) {

            switch(state) {

                case PARSER_STATE_NEW_LINE:
                    switch (*char_ptr) {
                        case '[':
                            state = PARSER_STATE_NAMESPACE;
                            buffer_ptr = command_buffer;
                            break;

                        case '\n':
                            line = line + 1;
                            break;

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
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
                            break;
                    }
                    break;

                case PARSER_STATE_NAMESPACE:
                    switch (*char_ptr) {
                        case ']':
                            state = PARSER_STATE_POST_NAMESPACE;
                            *buffer_ptr = '\0';
                            break;

                        case ' ':
                        case '\t':
                        case '#':
                            state = PARSER_STATE_ERROR;
                            error_msg = "Error in namespace.";
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
                            line = line + 1;
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
                            *buffer_ptr = '\0';
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

                        case '"':
                            state = PARSER_STATE_ARGUMENT_TEXT;
                            buffer_ptr = command_buffer;
                            break;

                        default:
                            state = PARSER_STATE_ARGUMENT;
                            buffer_ptr = command_buffer;
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
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
                            line = line + 1;
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
                            line = line + 1;
                            break;

                        case ' ':
                        case '\t':
                            /* ignore white space. */
                            break;

                        case '\\':
                            state = PARSER_STATE_POST_ARGUMENT_NEW_LINE;
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
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
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
                            line = line + 1;
                            break;

                        case ' ':
                        case '\t':
                            /* ignore white space. */
                            break;

                        default:
                            state = PARSER_STATE_ERROR;
                            error_msg = "Missing new line character.";
                            break;
                    }
                    break;

                case PARSER_STATE_ERROR:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            *buffer_ptr = '\0';
                            handler->func_error(handler->handler, filename,
                                                line, error_msg);
                            line = line + 1;
                            break;

                        default:
                            *buffer_ptr = *char_ptr;
                            buffer_ptr++;
                            break;
                    }
                    break;

                case PARSER_STATE_COMMENT:
                    switch (*char_ptr) {
                        case '\n':
                            state = PARSER_STATE_NEW_LINE;
                            line = line + 1;
                            *buffer_ptr = '\0';
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
                    break;
            }

            /* Get the next character. */
            char_ptr++;
        }
    }

    handler->func_end_config(handler->handler);

    fclose(file);
}




#ifdef _NOT_USED_
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
        config->name_space[0] = '\0';
        config->error_msg = NULL;
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
 * \return \c PARSER_OK if everything went well, otherwise return an error code.
 */
int config_parser_read(config_parser_t *config)
{
    char *command_pos_ptr;
    char *argument_pos_ptr;
    char *namespace_pos_ptr;
    int status = PARSER_NO_DATA;
    bool next_char = true;

    if (config == NULL) {
        return PARSER_MISSING_FILE;
    } else {
        command_pos_ptr = config->command;
        argument_pos_ptr = config->argument;
        namespace_pos_ptr = config->name_space;
        *command_pos_ptr = '\0';
        *argument_pos_ptr = '\0';
    }

    config->eof = false;
    config->error_msg = NULL;
    config->mode = NEW_LINE;
    config->argument_size = 0u;
    config->next_argument_pos_ptr = config->argument;

    while ((config->mode != EXIT) && (config->eof == false)) {

        config_parser_readfile(config);

        while (((config->buffer_pos < config->bytes_read) &&
               (config->mode != EXIT)) || (config->mode == ADD_COMMAND)) {

            switch(config->mode) {
                case NEW_LINE:
                    next_char = false;
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
                        case '\n':
                            config->mode = NEW_LINE;
                            break;
                        case ' ':
                        case '\t':
                            /* Do nothing. */
                            break;
                        case '[':
                            config->mode = NAMESPACE;
                            break;
                        case '#':
                            config->mode = COMMENT;
                            break;
                        default:
                            next_char = false;
                            config->mode = CHECK_COMMAND;
                            break;
                    }
                    break;

                case CHECK_COMMAND:
                    switch(*config->buffer_pos_ptr) {
                        case '=':
                            config->error_msg = MSG_MISSING_COMMAND;
                            config->mode = PRE_ERROR;
                            break;
                        default:
                            next_char = false;
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
                            config->mode = COMMENT_COMMAND;
                            break;
                        case '=':
                            config->mode = PRE_ARGUMENT;
                            break;
                        case ' ':
                        case '\t':
                            config->mode = POST_COMMAND;
                            break;
                        default:
                            if (command_pos_ptr <
                                (config->command + MAX_COMMAND)) {

                                *command_pos_ptr = *config->buffer_pos_ptr;
                                command_pos_ptr++;
                            } else {
                                config->error_msg = MSG_COMMAND_TO_LONG;
                                config->mode = PRE_ERROR;
                            }
                            break;
                    }
                    break;

                case POST_COMMAND:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;
                        case '=':
                            config->mode = PRE_ARGUMENT;
                            break;
                        case '#':
                            config->mode = COMMENT_COMMAND;
                            break;
                        case ' ':
                        case '\t':
                            /* Do nothing. */
                            break;
                        default:
                            config->error_msg = MSG_SPACE_NOT_SUPPORTED;
                            config->mode = PRE_ERROR;
                            break;
                    }
                    break;

                case PRE_ARGUMENT:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;
                        case ' ':
                        case '\t':
                            /* Do nothing. */
                            break;
                        case '\\':
                            config->mode = ARGUMENT_NEW_LINE;
                            break;
                        case '#':
                            config->mode = COMMENT_COMMAND;
                            break;
                        case '"':
                            config->mode = ARGUMENT_TEXT;
                            break;

                        default:
                            next_char = false;
                            config->mode = ARGUMENT;
                            break;
                    }
                    break;

                case ARGUMENT_NEW_LINE:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = PRE_ARGUMENT;
                            config->line++;
                            break;
                        default:
                            config->error_msg = MSG_EXPECTING_NEW_LINE;
                            config->mode = PRE_ERROR;
                            break;
                    }
                    break;

                case ARGUMENT:
                    switch(*config->buffer_pos_ptr) {
                        case '#':
                        case '\n':
                            next_char = false;
                            config->mode = ADD_ARGUMENT;
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

                                *argument_pos_ptr = *config->buffer_pos_ptr;
                                argument_pos_ptr++;
                            } else {
                                config->error_msg = MSG_ARGUMENT_TO_LONG;
                                config->mode = PRE_ERROR;
                            }
                            break;
                    }
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

                                *argument_pos_ptr = *config->buffer_pos_ptr;
                                argument_pos_ptr++;
                            } else {
                                config->error_msg = MSG_ARGUMENT_TO_LONG;
                                config->mode = PRE_ERROR;
                            }
                            break;
                    }
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
                                config->error_msg = MSG_COMMAND_TO_LONG;
                                config->mode = PRE_ERROR;
                            }
                            break;
                        default:
                            if (namespace_pos_ptr <
                                (config->name_space + MAX_COMMAND)) {

                                *namespace_pos_ptr = *config->buffer_pos_ptr;
                                namespace_pos_ptr++;
                            } else {
                                config->error_msg = MSG_COMMAND_TO_LONG;
                                config->mode = PRE_ERROR;
                            }
                            break;
                    }
                    break;

                case POST_NAMESPACE:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = NEW_LINE;
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
                    break;

                case COMMENT:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = NEW_LINE;
                            break;
                        default:
                            /* Do nothing. */
                            break;
                    }
                    break;

                case COMMENT_COMMAND:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->mode = ADD_COMMAND;
                            break;
                        default:
                            /* Do nothing. */
                            break;
                    }
                    break;

                case ADD_ARGUMENT:
                    next_char = false;
                    if (argument_pos_ptr < (config->argument + MAX_ARGUMENT)) {
                        *argument_pos_ptr = '\0';
                        argument_pos_ptr++;
                        config->argument_size++;
                        config->mode = PRE_ARGUMENT;
                    } else {
                        config->error_msg = MSG_COMMAND_TO_LONG;
                        config->mode = PRE_ERROR;
                    }
                    config->mode = PRE_ARGUMENT;
                    break;

                case ADD_COMMAND:
                    next_char = false;
                    if (command_pos_ptr != config->command) {
                        *command_pos_ptr = '\0';
                        *argument_pos_ptr = '\0';
                        status = PARSER_OK;
                    }
                    config->mode = EXIT;
                    break;

                case PRE_ERROR:
                default:
                    switch(*config->buffer_pos_ptr) {
                        case '\n':
                            config->argument[0] = '\0';
                            config->command[0] = '\0';
                            config->argument_size = 0u;
                            config->mode = EXIT;
                            status = PARSER_ERROR;
                            break;
                        default:
                            /* Do nothing. */
                            break;
                    }
                    break;
            }

            if (next_char) {
                config->buffer_pos_ptr++;
                config->buffer_pos++;
            }
            next_char = true;

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
    if (config != NULL) {
        strcpy(config->name_space, str);
    }
}

const char* config_parser_get_namespace(config_parser_t *config)
{
    if (config != NULL) {
        return config->name_space;
    } else {
        return NULL;
    }
}


const char* config_parser_get_command(config_parser_t *config)
{
    if (config != NULL) {
        return config->command;
    } else {
        return NULL;
    }
}

const char *config_parser_get_error_msg(config_parser_t *config)
{
    if (config != NULL) {
        return config->error_msg;
    } else {
        return NULL;
    }
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
    if (config != NULL) {
        return config->argument_size;
    } else {
        return 0;
    }
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
                config->mode = EXIT;
            } else {
                config->mode = ADD_COMMAND;
            }
            config->eof = true;
        }
    }
}
#endif
