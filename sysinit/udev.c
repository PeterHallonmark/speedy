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

#include "udev.h"
#include "config/udev.h"
#include "lib/run.h"
#include "lib/file.h"

#include <string.h>
#include <stdlib.h>

const char *udev_get_name(void)
{
    static const char priv_udev_name[] = "udev";
    
    return priv_udev_name;
}

void udev_init(void)
{    
    char *const udevadm_ctrl1_arg[] = {"/sbin/udevadm", "control", "--property=STARTUP=1", NULL};
    char *const udevadm_trigger_arg[] = {"/sbin/udevadm", "trigger", NULL};
    char *const udevadm_settle_arg[] = {"/sbin/udevadm", "settle", NULL};
    char *const udevadm_ctrl0_arg[] = {"/sbin/udevadm", "control", "--property=STARTUP=", NULL};
    char *const modprobe_arg[] = {"/sbin/modprobe", "ath5k", NULL};
    char *const udevd_arg[] = {"/sbin/udevd", "--daemon", NULL};
    
    file_empty("/proc/sys/kernel/hotplug");
    run("/sbin/udevd", udevd_arg);

    run("/sbin/udevadm", udevadm_ctrl1_arg);
    run("/sbin/udevadm", udevadm_trigger_arg);
    
	run("/sbin/modprobe", modprobe_arg);
    
    run("/sbin/udevadm", udevadm_settle_arg);
	run("/sbin/udevadm", udevadm_ctrl0_arg);
}
