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

#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int run(const char *filename, char *const argv[])
{
    char * const env[] = {NULL};
    pid_t pid = fork();
    int exec_return;
    int status = 0;
    
    /* Check if it is the child. */
    if (pid == 0) {
        exec_return = execve(filename, argv, env);
        /* If execvp fails. */
        printf("Failure! execv error code = %d for %s\n", exec_return, filename);

        exit(0); 
    } else if (pid < 0) {
        printf("Failed to fork\n");
    } else {
        /* Since it successfully ws able to fork, wait until the child
           process has run completly. */
        wait(&status);
    }
    if (status != 0) {
        printf("FAIL: %s\n",filename);
    }
    return status;
}

bool file_exists(const char *filename)
{
    FILE* file = fopen(filename, "rb");
    
    if (file != 0) {
        fclose(file);
        return true;
    }
    return false;
}

bool file_copy(const char *source, const char *destination)
{
    FILE *source_file;
    FILE *destination_file;
    char temp;
    
    /* open source file */
    source_file = fopen(source, "rb");
    if(source_file == NULL) {
        printf("Cannot open source file.\n");
        return false;
    }
    /* open destination file */
    destination_file = fopen(destination, "wb");
    if (destination_file == NULL) {
        printf("Cannot open destination file.\n");
        return false;
    }

    /* copy the file */
    while(!feof(source_file)) {
        temp = fgetc(source_file);
        
        if (ferror(source_file)) {
            printf("Error reading source file.\n");
            return false;
        }
        if (!feof(source_file)) {
            fputc(temp, destination_file);
        }
        if (ferror(destination_file)) {
            printf("Error writing destination file.\n");
            return false;
        }
    }

    if (fclose(source_file) == EOF) {
        printf("Error closing source file.\n");
        return false;
    }
    if (fclose(destination_file) == EOF) {
        printf("Error closing destination file.\n");
        return false;
    }    
    return true;
}
