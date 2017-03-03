#ifndef _UDP_H
#define _UDP_H

#include <netinet/in.h>

#define BUFFSIZE 131071 //TODO: try for the best performance

#define ETHERNET 0
#define WIGIG 1
#define WIFI 2

/*
struct addr_cs_pair
{
	struct sockaddr_in sendaddr;
	struct sockaddr_in recvaddr;
};
*/

int isipv4(char* ip);

void get_dstip(int iface_index);

int create_udp_fd();

void set_addr(struct sockaddr_in* addr, in_addr_t ip, int port);

void send_msg(int fd, char* msg, struct sockaddr_in* addr);

void recv_msg(int fd, char* msg, struct sockaddr_in* addr);

#endif
