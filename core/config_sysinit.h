/* This is a generated file. */

#include "../sysinit/hostname.h"
#include "../sysinit/keymap.h"


service_t sysinit[] = {
        {
            .init = &hostname_init,
            .shutdown = &hostname_shutdown
        }, {
            .init = &keymap_init,
            .shutdown = &keymap_shutdown
        }
    };
