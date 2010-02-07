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
#include "lib/run.h"
#include "lib/file.h"
#include "lib/command.h"

#include <stdlib.h>

#ifndef HWCLOCK_ADJTIME
#define HWCLOCK_ADJTIME "/var/lib/hwclock/adjtime"
#endif

#ifndef LOCALTIME
#define LOCALTIME "/etc/localtime"
#endif

const char *hwclock_get_name(void)
{
    static const char priv_hwclock_name[] = "hwclock";
    
    return priv_hwclock_name;
}

void hwclock_init(void)
{
#ifdef HWCLOCK_PARAMS    
    char *const hwclock1_arg[] = {CMD_HWCLOCK, "--adjust", NULL};    
    char *const hwclock2_arg[] = {CMD_HWCLOCK, HWCLOCK_PARAMS, NULL};    
#endif
    
    if (!file_exists(HWCLOCK_ADJTIME)) {
        file_write(HWCLOCK_ADJTIME, "0.0 0 0.0\n");
    }
#ifdef TIMEZONE    
    file_remove(LOCALTIME);
    file_copy("/usr/share/zoneinfo/"TIMEZONE, LOCALTIME);
#endif
#ifdef HWCLOCK_PARAMS    
    run(hwclock1_arg);
    run(hwclock2_arg);    
#endif
}
