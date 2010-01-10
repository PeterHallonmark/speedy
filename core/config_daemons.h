/* This is a generated file. */

#include "../daemons/alsa.h"
#include "../daemons/network.h"
#include "../daemons/samba.h"


service_t daemons[] = {
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
