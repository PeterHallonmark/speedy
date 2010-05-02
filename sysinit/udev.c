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
#include "lib/config.h"

#include <stdio.h>
#include <stdlib.h>

bool udev_modprobe(char * module);

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
    char *const udevd_arg[] = {CMD_UDEVD, "--daemon", NULL};
    int i = 0;
    
    libspeedy_file_empty(KERNEL_HOTPLUG);
    libspeedy_run(udevd_arg);

    libspeedy_run(udevadm_ctrl1_arg);
    libspeedy_run(udevadm_trigger_arg);
    
    while (modules[i] != 0) {
        udev_modprobe(modules[i]);
        i++;
    }
    
    libspeedy_run(udevadm_settle_arg);
	libspeedy_run(udevadm_ctrl0_arg);
}

bool udev_modprobe(char * module)
{
    char * modprobe_arg[] = {CMD_MODPROBE, module, NULL};

    printf("  modprobe %s\n",module);
    libspeedy_run(modprobe_arg);
    return true;
}
