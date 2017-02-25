#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "udp.h"
#include "console.h"

int main(int argc, char* argv[])
{
	if (argc != 3) {
		puts("Usage: ./client <destIP> <destPort>");
		exit(EXIT_FAILURE);
	}


	int nfds;
	int wififd;
	int wigigfd;
	int iface_index;
	fd_set afds;
	fd_set rfds;
	char msg[BUFFSIZE + 1];
	struct sockaddr_in svaddr;

	//create ip-binded UDP socket of client
	wigigfd = ipbinded_udp("wigig");
	wififd = ipbinded_udp("wifi");

	//initial server sockaddr struct
	set_addr(&svaddr, argv[1], atoi(argv[2]));

	//initial fd sets
	FD_ZERO(&afds);
	FD_SET(wififd, &afds);
	FD_SET(wigigfd, &afds);

	//initial number of fd set
	nfds = (wififd > wigigfd)? wififd + 1: wigigfd + 1;

	//initial screen
	init_console();

	//control handoff
	iface_index = ctl_iface();
	if (iface_index == 1)
		send_msg(wigigfd, "ready", &svaddr);
	else if (iface_index == 2)
		send_msg(wififd, "ready", &svaddr);

	while (1) {
		//select ready fd
		rfds = afds;
		if (select(nfds, &rfds, NULL, NULL, NULL) == -1)
			perror("select()");

		//recv msg from server
		if (FD_ISSET(wififd ,&rfds)) { //recv from iface wifi
			recv_msg(wififd, msg, &svaddr);
			show_info(msg, "wifi");
		} else { //recv from iface wigig
			recv_msg(wigigfd, msg, &svaddr);
			show_info(msg, "wigig");
		}
	}
}
