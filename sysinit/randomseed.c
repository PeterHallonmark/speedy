#include "randomseed.h"
#include "config/randomseed.h"

#ifdef SIMULATE
#include "lib/simulate.h"
#else
#include <stdio.h>
#endif

void randomseed_init(void)
{
    system("cat " RANDOMSEED " > /dev/urandom");
}

void randomseed_shutdown(void)
{
}
