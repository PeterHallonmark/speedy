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
#include "subject.h"
#include "task.h"

#include <stdlib.h>

task_t * task_init(service_t *service)
{
    task_t * this_ptr = (task_t*) malloc(sizeof(task_t));
    subject_init(&this_ptr->task);
    this_ptr->service = service;
    return task;
}

const char * task_get_name(task_t *this_ptr)
{
    if (this_ptr->service->get_name != NULL) {
        return this_ptr->service->get_name();
    }
    return NULL;
}

const char * task_provides(task_t *this_ptr)
{
    if (this_ptr->service->provides != NULL) {
        return this_ptr->service->provides();
    }
    return NULL;
}

bool task_run(task_t *this_ptr)
{
    if (this_ptr->service->initialization != NULL) {
        this_ptr->service->initialization();
    }
}

void task_deinit(task_t *this_ptr)
{
    subject_deinit(&this_ptr->task);
    free(this_ptr);
}
