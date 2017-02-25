#ifndef _UDP_H
#define _UDP_H

#define BUFFSIZE 1023

int sv_udp(int port);

void send_msg(int fd, char* msg, struct sockaddr_in* addr);

void recv_msg(int fd, char* msg, struct sockaddr_in* addr);

#endif

