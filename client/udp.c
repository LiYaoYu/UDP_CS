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

in_addr_t get_bindip(char* iface)
{
	char bindip[INET_ADDRSTRLEN];

	while (1) {
		printf("Please input the IP of %s interface.\n", iface);
		scanf("%s", bindip);

		if (isipv4(bindip)) {
			return inet_addr(bindip);
		}
		puts("Invalid IP.");
	}
}

int ipbinded_udp(char* iface)
{
	int fd;
	int optval;
	struct sockaddr_in claddr;

	claddr.sin_family = AF_INET;

	if ((fd = socket(claddr.sin_family, SOCK_DGRAM, 0)) == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	
	while (1) {
		claddr.sin_addr.s_addr = get_bindip(iface);

		if (bind(fd, (struct sockaddr*)&claddr, sizeof(claddr)) == -1) {
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

void set_addr(struct sockaddr_in* addr, char* ip, int port)
{
	//set sockaddr_in struct
	(*addr).sin_family = AF_INET;
	(*addr).sin_port = htons(port);
	(*addr).sin_addr.s_addr = inet_addr(ip);
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
