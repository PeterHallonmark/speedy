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
#include "lib/config.h"

#include <stdlib.h>

const char *hwclock_get_name(void)
{
    static const char priv_hwclock_name[] = "hwclock";
    
    return priv_hwclock_name;
}

int hwclock_initialization(void)
{
#ifdef HWCLOCK_PARAMS    
    char *const hwclock1_arg[] = {CMD_HWCLOCK, "--adjust", NULL};
    char *const hwclock2_arg[] = {CMD_HWCLOCK, HWCLOCK_PARAMS, NULL};
#endif
    
    if (!libspeedy_file_exists(FILE_HWCLOCK_ADJTIME)) {
        libspeedy_file_write(FILE_HWCLOCK_ADJTIME, "0.0 0 0.0\n");
    }
#ifdef TIMEZONE    
    libspeedy_file_remove(FILE_LOCALTIME);
    libspeedy_file_copy(PATH_ZONEINFO"/"TIMEZONE, FILE_LOCALTIME);
#endif
#ifdef HWCLOCK_PARAMS
    libspeedy_run(hwclock1_arg);
    libspeedy_run(hwclock2_arg);
#endif
    return 0;
}
