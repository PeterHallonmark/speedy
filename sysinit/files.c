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

#include "files.h"
#include "lib/file.h"
#include "lib/dir.h"
#include "lib/config.h"

const char *files_get_name(void)
{
    static const char priv_files_name[] = "files";
    
    return priv_files_name;
}

void files_init(void)
{    
    file_remove(FILE_NOLOGIN);
    file_remove(FILE_SHUTDOWNPID);
    file_remove_all(PATH_LOCK, false, false);
    file_remove_all(PATH_TMP, true, true);
    file_remove(FILE_FORCECHK);
    file_remove_all(PATH_RUN, true, false);
    file_empty(FILE_UTMP);
    file_chmod(FILE_UTMP, 0664);
    dir_mkdir(DIR_TMP_ICE, 1777);
    dir_mkdir(DIR_TMP_X11, 1777);
}
