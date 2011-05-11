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

#include "str_ext.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*!
 * Duplicates a string.
 * This could have been done with strdup function from string.h but it is not
 * part of the c99 standard.
 *
 * \param source - The source string that is going to be duplicated.
 *
 * \return The duplicated string.
 */
char * strdup_ext(const char *source)
{
    char *destination = NULL;

    destination = malloc(strlen(source) + 1);
    if (destination != NULL) {
        strcpy(destination, source);
    }
    return destination;
}
