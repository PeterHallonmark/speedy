#include "randomseed.h"
#include "config/randomseed.h"
#include "lib/run.h"
#include <stdio.h>


void randomseed_init(void)
{
    system("cat " RANDOMSEED " /dev/urandom");
}

void randomseed_shutdown(void)
{
}
