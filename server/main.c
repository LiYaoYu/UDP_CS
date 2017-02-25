#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "udp.h"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		puts("Usage: ./server <port>");
		exit(EXIT_FAILURE);
	}

	int svfd;
	int seq_num;
	char msg[BUFFSIZE + 1];
	char cl_ip[INET_ADDRSTRLEN];
	struct sockaddr_in claddr;

	//get server socket fd
	svfd = sv_udp(atoi(argv[1]));

	//recv msg from client
	recv_msg(svfd, msg, &claddr);

	//show recv msg info
	printf("Recvfrom %s: %s\n", inet_ntop(claddr.sin_family,\
		&claddr.sin_addr.s_addr, cl_ip, sizeof(cl_ip)), msg);

	//send sequence number to client
	seq_num = 0;
	while (1) {
		snprintf(msg, sizeof(msg), "%d", seq_num++);

		send_msg(svfd, msg, &claddr);

		sleep(1);
	}
}
