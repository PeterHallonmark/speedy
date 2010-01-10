#include "config_daemons.h"
#include <stdio.h>

int main(void)
{
	int size = sizeof(daemons) / sizeof(daemon_t);
	int i;
	
	for (i = 0; i < size; i++) {
		daemons[i].init();
	}
	printf("\n");
	
	for (i = size - 1; i >= 0; i--) {
		daemons[i].shutdown();
	}
	
	return 0;
}
