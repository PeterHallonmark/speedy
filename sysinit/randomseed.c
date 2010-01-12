#include "randomseed.h"
#include "config/randomseed.h"
#include "../lib/run.h"
#include <stdio.h>


void randomseed_init(void)
{
    char* const argv[] = {RANDOMSEED, ">", "/dev/urandom", NULL};
    run("/bin/cat", argv);
}

void randomseed_shutdown(void)
{
}
