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

#include "hwclock.h"
#include "config/hwclock.h"
#include "lib/functions.h"

#include <string.h>
#include <stdlib.h>

const char *hwclock_get_name(void)
{
    static const char priv_hwclock_name[] = "hwclock";
    
    return priv_hwclock_name;
}

void hwclock_init(void)
{
#ifdef TIMEZONE    
    char *const rm_arg[] = {"/bin/rm", "-f", "/etc/localtime", NULL};
#endif
#ifdef HWCLOCK_PARAMS    
    char *const hwclock1_arg[] = {"/sbin/hwclock", "--adjust", NULL};    
    char *const hwclock2_arg[] = {"/sbin/hwclock", HWCLOCK_PARAMS, NULL};    
#endif
    
    if (!file_exists("/var/lib/hwclock/adjtime")) {
        system("echo \"0.0 0 0.0\" > /var/lib/hwclock/adjtime");
    }
#ifdef TIMEZONE    
    run("/bin/rm", rm_arg);
    file_copy("/usr/share/zoneinfo/"TIMEZONE, "/etc/localtime");
#endif
#ifdef HWCLOCK_PARAMS    
    run("/sbin/hwclock", hwclock1_arg);
    run("/sbin/hwclock", hwclock2_arg);    
#endif
}
