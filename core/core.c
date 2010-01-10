#include "core_type.h"
#include "config_sysinit.h"
#include "config_daemons.h"
#include <stdio.h>

int main(void)
{
	int daemon_size = sizeof(daemons) / sizeof(service_t);
	int sysinit_size = sizeof(sysinit) / sizeof(service_t);
	int i;
	
	for (i = 0; i < sysinit_size; i++) {
		sysinit[i].init();
	}
	for (i = 0; i < daemon_size; i++) {
		daemons[i].init();
	}
	printf("\n");
	
	for (i = daemon_size - 1; i >= 0; i--) {
		daemons[i].shutdown();
	}
	
	return 0;
}
