#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>

#include "udp.h"

int isipv4(char* ip)
{
	if (inet_addr(ip) == INADDR_NONE)
		return 0;

	int i = 0;
	int cnt = 0;

	while (ip[i]) {
		if (ip[i] == '.')
			cnt++;

		i++;
	}
	
	//bind ip cannot be 0.0.0.0 because it may bind to any interface
	if (cnt == 3 && strncmp(ip, "0.0.0.0", sizeof("0.0.0.0") != 0))
		return 1;

	return 0;
}

void get_dstip(int iface_index)
{
	extern char dstip[2][INET_ADDRSTRLEN];

	while (1) {
		if (iface_index == ETHERNET)
			puts("Please input dst server IP.");
		else if (iface_index == WIGIG)
			puts("Please input dst wigig interface IP.");
		else if (iface_index == WIFI)
			puts("Please input dst wifi interface IP.");

		scanf("%s", dstip[iface_index]);

		if (isipv4(dstip[iface_index])) {
			return;
		}
		puts("Invalid IP.");
	}
}

int create_udp_fd(int port)
{
	int fd;
	int optval;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if ((fd = socket(addr.sin_family, SOCK_DGRAM, 0)) == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	
	while (1) {
		if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			perror("bind()");
			continue;
		}

		optval = 1;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval,\
				sizeof(optval)) == -1)
			perror("setsockopt()");
		else
			break;
	}

	return fd;
}

void set_addr(struct sockaddr_in* addr, in_addr_t ip, int port)
{
	//set sockaddr_in struct
	(*addr).sin_family = AF_INET;
	(*addr).sin_port = htons(port);
	(*addr).sin_addr.s_addr = ip;
}

int recv_msg(int fd, char* msg, struct sockaddr_in* addr)
{
	int n = 0;
	socklen_t addr_len = sizeof(*addr);

	if ((n = recvfrom(fd, msg, BUFFSIZE, 0,\
			 (struct sockaddr*)addr, &addr_len)) == -1)
		perror("recvfrom()");

	return n;
}

void send_msg(int fd, char* msg, int nbytes, struct sockaddr_in* addr)
{
	if (sendto(fd, msg, nbytes, 0, (struct sockaddr*)addr,\
			sizeof(*addr)) == -1)
		perror("sendto()");
}
