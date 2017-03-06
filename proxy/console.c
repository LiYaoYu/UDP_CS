#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "console.h"

void show_info(char* msg, char* iface)
{
	//show the handoff information
	printf("\033[7;1H");
	printf("handoff: ");
	fflush(stdout);
}

void init_console()
{
	extern char dstip[3][INET_ADDRSTRLEN];

	//terminal anivation
	printf("\033[2J");
	printf("\033[H");
	
	//show cmd of handoff between two IP
	printf("1: %s\n2: %s", dstip[1], dstip[2]);

	show_info(" ", " ");
}

int ctl_iface()
{
	int index;
	extern char dstip[3][INET_ADDRSTRLEN];

	while (1) {
		scanf("%d", &index);
		if (index == 1 || index == 2) {
			printf("\033[8;1H");
			printf("Handoff to %s", dstip[index]);
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
