#ifndef _UDP_H
#define _UDP_H

#include <netinet/in.h>

#define BUFFSIZE 1023

struct socketfds
{
	int fd_1;
	int fd_2;
};

int isipv4(char* ip);

in_addr_t get_bindip(char* iface);

int ipbinded_udp(char* iface);

void set_addr(struct sockaddr_in* addr, char* ip, int port);

void send_msg(int fd, char* msg, struct sockaddr_in* addr);

void recv_msg(int fd, char* msg, struct sockaddr_in* addr);

#endif
