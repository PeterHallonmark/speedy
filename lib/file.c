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

#include "file.h"
#include "config.h"
#include "dir.h"

#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUF_SIZE 512

bool libspeedy_file_exists(const char *filename)
{
    FILE* file = fopen(filename, "rb");
    
    if (file != 0) {
        fclose(file);
        return true;
    }
    return false;
}

bool libspeedy_file_copy(const char *source, const char *destination)
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

bool libspeedy_file_copy_all(const char *source_path, const char *dest_path, bool recursive)
{
    DIR *dir;
    struct dirent *content;
    struct stat stbuf;    
    char next_source[BUF_SIZE];
    char next_dest[BUF_SIZE];

    
    dir = opendir(source_path);
    if (dir)  
    {
        if (recursive) {
            libspeedy_mkdir(dest_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        
        while ((content = readdir(dir)) != NULL)
        {            

            if ((snprintf(next_source, BUF_SIZE, "%s/%s", source_path, content->d_name) < BUF_SIZE) &&
                (snprintf(next_dest, BUF_SIZE, "%s/%s", dest_path, content->d_name) < BUF_SIZE)) {
    
                stat(next_source, &stbuf);
                
                if (S_ISDIR(stbuf.st_mode)) {

                    if ((strcmp(content->d_name, ".")  != 0) &&
                        (strcmp(content->d_name, "..")  != 0)) { 
                    
                        if (recursive) {
                            libspeedy_file_copy_all(next_source, next_dest, true);
                        }
                    }    
                } else {
                    if (!libspeedy_file_copy(next_source, next_dest)) {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        closedir(dir);
    }
    return true;
}

bool libspeedy_file_empty(const char *destination)
{
    FILE *destination_file;
    
    /* open destination file */
    destination_file = fopen(destination, "wb");
    if (destination_file == NULL) {
        printf("Cannot open destination file.\n");
        return false;
    }
    if (fclose(destination_file) == EOF) {
        printf("Error closing destination file.\n");
        return false;
    }    
    return true;
}


bool libspeedy_file_remove(const char *destination)
{
    remove(destination);
    
    return true;
}


bool libspeedy_file_remove_all(const char *dest_path, bool recursive, bool remove_dir)
{
    DIR *dir;
    struct dirent *content;
    struct stat stbuf;    
    char next[BUF_SIZE];
        
    dir = opendir(dest_path);
    if (dir) {
        while ((content = readdir(dir)) != NULL) {            

            if (snprintf(next, BUF_SIZE, "%s/%s", dest_path, content->d_name) < BUF_SIZE) {
                stat(next, &stbuf);
                
                if (S_ISDIR(stbuf.st_mode)) {

                    if ((strcmp(content->d_name, ".")  != 0) &&
                        (strcmp(content->d_name, "..")  != 0)) { 
                    
                        if (recursive) {
                            libspeedy_file_remove_all(next, true, remove_dir);
                            if (remove_dir) {
                                libspeedy_rmdir(next);
                            }
                        }
                    }    
                } else {
                    if (!libspeedy_file_remove(next)) {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        closedir(dir);
    }
    
    return true;
}

bool libspeedy_file_chmod(const char *pathname, int mode)
{
    chmod(pathname, mode);
    
    return true;
}

bool libspeedy_file_write(const char *filename, const char *text)
{
    FILE* file = fopen(filename, "w+");
    
    if (file == NULL) {
        return false;
    }
    fwrite(text, 1, strlen(text), file);
    fclose(file);
    
    return true;
}

bool libspeedy_file_tty_action(bool (*callback)(char *filename))
{
    DIR *dir;
    struct dirent *content;
    char buffer[BUF_SIZE];

    dir = opendir(PATH_DEV);
    if (dir) {
        while ((content = readdir(dir)) != NULL) {

            if ((strcmp(content->d_name, "tty0") >= 0) &&
                (strcmp(content->d_name, "tty9999") <=  0)) {

                if (snprintf(buffer, BUF_SIZE, "%s/%s", PATH_DEV, content->d_name) < BUF_SIZE) {
                    if (!callback(buffer)) {
                        return false;
                    }
                }
            }
        }
        closedir(dir);
    }

    return true;
}
