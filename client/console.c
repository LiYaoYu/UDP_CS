#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "console.h"

void show_info(char* msg, char* iface)
{
	//show the server response
	printf("\033[4;1H");
	printf("msg from server: %s", msg);
	printf("\033[5;1H");
	printf("msg from iface: %s", iface);
	printf("\033[7;1H");
	printf("handoff: ");
	fflush(stdout);
}

void init_console()
{
	//terminal anivation
	printf("\033[2J");
	printf("\033[H");
	
	//show cmd of handoff between wigig and wifi
	puts("1: Wigig\n2: Wifi");

	show_info(" ", " ");
}

int ctl_iface()
{
	int index;

	while (1) {
		scanf("%d", &index);
		if (index == 1 || index == 2) {
			printf("\033[8;1H");
			printf("Handoff to %d              ", index);
			printf("\033[7;1H");
			printf("handoff: ");
			break;
		} else {
			printf("\033[8;1H");
			printf("Only 1 or 2 is acceptable.");
			printf("\033[7;1H");
			printf("handoff: ");
		}
	}
	
	return index;
}
