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
#include "lib/functions.h"

#include <stdlib.h>

const char *files_get_name(void)
{
    static const char priv_files_name[] = "files";
    
    return priv_files_name;
}

void files_init(void)
{    
    system("/bin/rm -f /etc/nologin &>/dev/null");
    system("/bin/rm -f /etc/shutdownpid &>/dev/null");
    system("/bin/rm -f /var/lock/* &>/dev/null");
    system("/bin/rm -rf /tmp/* /tmp/.* &>/dev/null");
    system("/bin/rm -f /forcefsck &>/dev/null");
    system("(cd /var/run && /usr/bin/find . ! -type d -exec /bin/rm -f -- {} \\; )"); 
    file_empty("/var/run/utmp");
    system("/bin/chmod 0664 /var/run/utmp");
    system("/bin/mkdir /tmp/.ICE-unix && /bin/chmod 1777 /tmp/.ICE-unix");
    system("/bin/mkdir /tmp/.X11-unix && /bin/chmod 1777 /tmp/.X11-unix");
}
