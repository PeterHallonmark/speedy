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

#include "run.h"

#include <stdio.h>
#include <sys/types.h>

#ifdef SIMULATE
#include "simulate.h"
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

void run(const char *filename, char *const argv[])
{
    pid_t pid = fork();
    int exec_return;
    int status;
    
    /* Check if it is the child. */
    if (pid == 0) {
        exec_return = execvp(filename, argv);
#ifndef SIMULATE
        /* If execvp fails. */
        printf("Failure! execv error code = %d\n", exec_return);
        exit(0); 
    } else if (pid < 0) {
        printf("Failed to fork\n");
    } else {
        /* Since it successfully ws able to fork, wait until the child
           process has run completly. */
        wait(&status);
#endif
    }
}
