#include "hostname.h"
#include "config/hostname.h"
#include <string.h>

#ifdef SIMULATE
#include "lib/simulate.h"
#else
#include <unistd.h>
#endif

void hostname_init(void)
{
    sethostname(hostname, strlen(hostname)); 
}

void hostname_shutdown(void)
{
}
