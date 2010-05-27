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

#include "hash_lookup.h"
#include "task_lookup.h"

#include <stdlib.h>

task_lookup_t * task_lookup_create(void)
{
    task_lookup_t * this_ptr = (task_lookup_t*) malloc(sizeof(task_lookup_t));

    if (this_ptr != NULL) {
        this_ptr->lookup = hash_lookup_create(64);

        if (this_ptr->lookup == NULL) {
            free(this_ptr);
            this_ptr = NULL;
        }
    }
    return this_ptr;
}

int task_lookup_insert(task_lookup_t *this_ptr, unsigned int key, task_t * task)
{
    int status;
    /* TODO Add lock. */
    status = hash_lookup_insert(this_ptr->lookup, key, task);
    /* TODO Add unlock. */
    return status;
}

task_t * task_lookup_remove(task_lookup_t *this_ptr, unsigned int key)
{
    task_t *task;

    /* TODO Add lock. */
    task = hash_lookup_remove(this_ptr->lookup, key);
    /* TODO Add unlock. */
    return task;
}

task_t * task_lookup_find(task_lookup_t *this_ptr, unsigned int key)
{
    task_t *task;

    /* TODO Add lock. */
    task = hash_lookup_find(this_ptr->lookup, key);
    /* TODO Add unlock. */
    return task;
}

void task_lookup_destroy(task_lookup_t *this_ptr)
{
    task = hash_lookup_destroy(this_ptr->lookup);
    free(this_ptr);
}
