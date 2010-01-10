/* This is a generated file. */

#include "core_type.h"
#include "../daemons/alsa.h"
#include "../daemons/network.h"
#include "../daemons/samba.h"


daemon_t daemons[] = {
        {
            .init = &alsa_init,
            .shutdown = &alsa_shutdown
        }, {
            .init = &network_init,
            .shutdown = &network_shutdown
        }, {
            .init = &samba_init,
            .shutdown = &samba_shutdown
        }
    };
