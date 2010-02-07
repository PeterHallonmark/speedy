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

#include "start.h"
#include "config/start.h"
#include "lib/run.h"
#include "lib/command.h"

#include <sys/mount.h> 
#include <stdlib.h>

const char *start_get_name(void)
{
    static const char priv_start_name[] = "start";
    
    return priv_start_name;
}

void start_init(void)
{    
    char *const minilogd_arg[] = {CMD_MINILOGD, NULL};
    char *const dmesg_arg[] = {CMD_DMESG, "-n", "3", NULL};
    char *const mknod_arg[] = {CMD_MKNOD, "/dev/rtc0", "c", RTC_MAJOR, "0", NULL};
    char *const ln_arg[] = {CMD_LN, "-s", "/dev/rtc0", "/dev/rtc", NULL};
    
    mount("udev", "/dev", "tmpfs", MS_NOSUID, "mode=0755,size=10M");
    mount("none", "/proc", "proc", 0, NULL);
    mount("none", "/sys", "sysfs", 0, NULL);

    /* Copy static device nodes to /dev */
    system("/bin/cp -a /lib/udev/devices/* /dev/");

    /* start up mini logger until syslog takes over */
    run(CMD_MINILOGD, minilogd_arg);

    run(CMD_DMESG, dmesg_arg);

    system(CMD_MODPROBE " rtc-cmos >/dev/null 2>&1");
    run(CMD_MKNOD, mknod_arg);
    run(CMD_LN, ln_arg);
}
