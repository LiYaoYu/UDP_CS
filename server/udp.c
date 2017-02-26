#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "udp.h"

int sv_udp(int port)
{
	int fd;
	struct sockaddr_in svaddr;

	//set sockaddr_in struct
	svaddr.sin_family = AF_INET;
	svaddr.sin_port = htons(port);
	svaddr.sin_addr.s_addr = INADDR_ANY;

	if ((fd = socket(svaddr.sin_family, SOCK_DGRAM, 0)) == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	
	if (bind(fd, (struct sockaddr*)&svaddr, sizeof(svaddr)) == -1) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	return fd;
}

void send_msg(int fd, char* msg, struct sockaddr_in* addr)
{
	if (sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)addr,\
			sizeof(*addr)) == -1)
		perror("sendto()");
}

void recv_msg(int fd, char* msg, struct sockaddr_in* addr)
{
	int numbytes;
	socklen_t addr_len = sizeof(*addr);

	if ((numbytes = recvfrom(fd, msg, sizeof(msg), 0,\
			 (struct sockaddr*)addr, &addr_len)) == -1)
		perror("recvfrom()");

	msg[numbytes] = '\0';
}

void listen_fd(int svfd)
{
	extern struct sockaddr_in claddr;

	char msg[BUFFSIZE + 1];
	char cl_ip[INET_ADDRSTRLEN];

	//recv msg from client
	recv_msg(svfd, msg, &claddr);

	//show recv msg info
	printf("Recvfrom %s: %s\n", inet_ntop(claddr.sin_family,\
		&claddr.sin_addr.s_addr, cl_ip, sizeof(cl_ip)), msg);
}
