#include "hostname.h"
#include "config/hostname.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void hostname_init(void)
{
    
    sethostname(hostname, strlen(hostname)); 
}

void hostname_shutdown(void)
{
}
