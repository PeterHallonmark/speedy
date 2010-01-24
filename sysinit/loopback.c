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

#include "loopback.h"
#include <string.h>
#include <stdlib.h>

#ifdef SIMULATE
#include "lib/simulate.h"
#else
#include "lib/run.h"
#endif

const char *loopback_get_name(void)
{
    static const char priv_loopback_name[] = "loopback";
    
    return priv_loopback_name;
}

void loopback_init(void)
{    
    char *const loopback_arg[] = {"/sbin/ifconfig" "lo" "127.0.0.1" "up", NULL};
    
    /* bring up the loopback interface */
    run("/sbin/ifconfig", loopback_arg);
}
