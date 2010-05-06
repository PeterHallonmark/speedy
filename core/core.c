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
#include "config_sysinit.h"
#include "config_daemons.h"

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>


pthread_t start;

void *thread_function1(void *dummyPtr)
{
    pthread_join(start, NULL);
    printf("Pthread test1\n");

    return 0;
}

void *thread_function2(void *dummyPtr)
{
    pthread_join(start, NULL);
    printf("Pthread test2\n");

    return 0;
}


void *start_function(void *dummyPtr)
{
    char ch;

    scanf("%c",&ch);
    printf("Start1\n");

    return 0;
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    int daemon_size = sizeof(daemons) / sizeof(service_t);
    int sysinit_size = sizeof(sysinit) / sizeof(service_t);
    const char **dependency;
    int i;

    pthread_create( &start, NULL, start_function, NULL);

    for (i = 0; i < sysinit_size; i++) {
        if (sysinit[i].get_name != NULL) {
            printf("%s\n",sysinit[i].get_name());
        }
        if (sysinit[i].get_dependency != NULL) {
            printf("  dep: ");
            dependency = sysinit[i].get_dependency();
            while (*dependency != NULL) {
                printf("%s ",*dependency);
                dependency++;
            }
            printf("\n");
        }
        if (sysinit[i].initialization != NULL) {
            sysinit[i].initialization();
        }

    }

    pthread_create( &thread1, NULL, thread_function1, NULL);
    pthread_create( &thread2, NULL, thread_function2, NULL);




    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Pthread join\n");

    return 0;
}
