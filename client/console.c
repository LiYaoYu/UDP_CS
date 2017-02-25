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
	fflush(stdout);
}

void init_console()
{
	//int flag;

	//terminal anivation
	printf("\033[2J");
	printf("\033[H");
	
	//show cmd of handoff between wigig and wifi
	puts("1: Wigig\n2: Wifi");

	show_info(" ", " ");

	//set stdin to non block
	/*
	flag = fcntl(STDIN_FILENO, F_GETFL, 0);
	if (fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK) == -1)
		perror("fcntl()");
	*/
}

int ctl_iface()
{
	static int index;

	printf("\033[7;1H");
	printf("handoff: ");

	while (1) {
		scanf("%d", &index);
		if (index == 1 || index == 2) {
			break;
		}
	}
	
	return index;
}
