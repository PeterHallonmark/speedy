#include "randomseed.h"
#include "config/randomseed.h"
#include "lib/run.h"
#include <stdio.h>


void randomseed_init(void)
{
/** \todo This needs a shell interpreter to work. (problem with pipes). */
#ifdef TEMPORARY_REMOVED
    char* const argv[] = {RANDOMSEED, ">", "/dev/urandom", NULL};
    run("/bin/cat", argv);
#endif
}

void randomseed_shutdown(void)
{
}
