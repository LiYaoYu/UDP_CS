#ifndef _UDP_H
#define _UDP_H

#include <netinet/in.h>

#define BUFFSIZE 131071

#define ETHERNET 0
#define WIGIG 1
#define WIFI 2

int isipv4(char* ip);

void get_dstip(int iface_index);

int create_udp_fd();

void set_addr(struct sockaddr_in* addr, in_addr_t ip, int port);

int recv_msg(int fd, char* msg, struct sockaddr_in* addr);

void send_msg(int fd, char* msg, int nbytes, struct sockaddr_in* addr);

#endif
