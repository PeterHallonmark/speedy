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
#include "config/files.h"
#include "lib/file.h"
#include "lib/dir.h"

const char *files_get_name(void)
{
    static const char priv_files_name[] = "files";
    
    return priv_files_name;
}

void files_init(void)
{    
    file_remove("/etc/nologin");
    file_remove("/etc/shutdownpid");
    file_remove_all("/var/lock", false, false);
    file_remove_all("/tmp", true, true);    
    file_remove("/forcefsck");
    file_remove_all("/var/run", true, false); 
    file_empty("/var/run/utmp");
    file_chmod("/var/run/utmp", 0664);
    dir_mkdir("/tmp/.ICE-unix", 1777);
    dir_mkdir("/tmp/.X11-unix", 1777);
}
