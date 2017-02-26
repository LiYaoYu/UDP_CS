#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "udp.h"
#include "console.h"

int iface_index;
struct sockaddr_in svaddr;

void *handle_fds(void *arg);

int main(int argc, char* argv[])
{
	if (argc != 3) {
		puts("Usage: ./client <destIP> <destPort>");
		exit(EXIT_FAILURE);
	}


	struct socketfds sfds;
	pthread_t t_handle_fds;


	//create ip-binded UDP socket of client
	sfds.fd_1 = ipbinded_udp("wigig");
	sfds.fd_2 = ipbinded_udp("wifi");

	//initial server sockaddr struct
	set_addr(&svaddr, argv[1], atoi(argv[2]));

	//initial screen
	init_console();

	//create thread to listen and handle fds
	pthread_create(&t_handle_fds, NULL, handle_fds, (void *)&sfds);

	//control handoff
	while (1) {
		iface_index = ctl_iface();
		if (iface_index == 1)
			send_msg(sfds.fd_1, "ready", &svaddr);
		else if (iface_index == 2)
			send_msg(sfds.fd_2, "ready", &svaddr);
	}
}

void *handle_fds(void *arg)
{
	struct socketfds sfds = *((struct socketfds *)arg);

	int nfds;
	fd_set afds;
	fd_set rfds;
	char msg[BUFFSIZE + 1];

	nfds = (sfds.fd_2 > sfds.fd_1)? sfds.fd_2 + 1: sfds.fd_1 + 1;

	//initial fd sets
	FD_ZERO(&afds);
	FD_SET(sfds.fd_2, &afds);
	FD_SET(sfds.fd_1, &afds);

	while (1) {
		rfds = afds;

		//select ready fd
		if (select(nfds, &rfds, NULL, NULL, NULL) == -1)
			perror("select()");

		//recv msg from server
		if (FD_ISSET(sfds.fd_2 ,&rfds)) { //recv from iface wifi
			recv_msg(sfds.fd_2, msg, &svaddr);
			show_info(msg, "wifi");
		} else { //recv from iface wigig
			recv_msg(sfds.fd_1, msg, &svaddr);
			show_info(msg, "wigig");
		}
	}
}
