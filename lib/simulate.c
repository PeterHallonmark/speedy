
#include "simulate.h"
#include <stdio.h>


void sethostname(const char* hostname, int size)
{
    printf("Hostname = %s\n", hostname);
}

void system(const char *command)
{
    printf("Command = %s\n", command);
}

int execv(const char *path, char *const argv[])
{
    printf("execv = %s",path);
    return 0;
}

int fork(void)
{
    return 0;
}

void wait(int *status)
{
}