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
#include "lib/command.h"

#include <stdlib.h>

#ifndef KERNEL_HOTPLUG
#define KERNEL_HOTPLUG "/proc/sys/kernel/hotplug"
#endif

const char *udev_get_name(void)
{
    static const char priv_udev_name[] = "udev";
    
    return priv_udev_name;
}

void udev_init(void)
{    
    char *const udevadm_ctrl1_arg[] = {CMD_UDEVADM, "control", "--property=STARTUP=1", NULL};
    char *const udevadm_trigger_arg[] = {CMD_UDEVADM, "trigger", NULL};
    char *const udevadm_settle_arg[] = {CMD_UDEVADM, "settle", NULL};
    char *const udevadm_ctrl0_arg[] = {CMD_UDEVADM, "control", "--property=STARTUP=", NULL};
    char *const modprobe_arg[] = {CMD_MODPROBE, "ath5k", NULL};
    char *const udevd_arg[] = {CMD_UDEVD, "--daemon", NULL};
    
    file_empty(KERNEL_HOTPLUG);
    run(udevd_arg);

    run(udevadm_ctrl1_arg);
    run(udevadm_trigger_arg);
    
	run(modprobe_arg);
    
    run(udevadm_settle_arg);
	run(udevadm_ctrl0_arg);
}
