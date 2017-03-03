#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "udp.h"
#include "console.h"

int dst_idx;
char buffer[BUFFSIZE + 1];
char dstip[3][INET_ADDRSTRLEN]; //0 for wigig, 1 for wifi

void *forward_helper(void *arg);
//void *forward_helper();

int main(int argc, char* argv[])
{
	if (argc != 2) {
		puts("Usage: ./client <listenPort>");
		exit(EXIT_FAILURE);
	}

	//int connfd;
	int port = atoi(argv[1]);
	pthread_t t_traffic;

	//create active UDP socket for sending
	//connfd = create_udp_fd();

	//initial listen sockaddr struct
	//set_addr(&svaddr, atoi(argv[1]));

	//get dst ip
	get_dstip(ETHERNET); //for ethernet
	get_dstip(WIGIG); //for wigig
	get_dstip(WIFI); //for wifi

	//initial screen
	init_console();

	//create thread to transmit packet
	pthread_create(&t_traffic, NULL, forward_helper, (void *)&port);
	//pthread_create(&t_traffic, NULL, forward_helper, NULL);

	//control handoff
	while (1) {
		dst_idx = ctl_iface(); 

		/*
		if (dst_idx == 1)
			send_msg(sfds.fd_1, "ready", &svaddr);
		else if (dst_idx == 2)
			send_msg(sfds.fd_2, "ready", &svaddr);
		*/
	}
}

void *forward_helper(void *arg)
//void *forward_helper()
{
	//int connfd = *((int *)&arg);
	int connfd;
	connfd = create_udp_fd(5001);
	in_addr_t ip_recv; //the binary recv ip
	struct sockaddr_in listen_addr;
	struct sockaddr_in sv_addr;
	struct sockaddr_in cl_addr;

	in_addr_t ip_0 = inet_addr(dstip[0]); //ethernet
	in_addr_t ip_1 = inet_addr(dstip[1]); //wigig
	in_addr_t ip_2 = inet_addr(dstip[2]); //wifi

	while (1) {
		recv_msg(connfd, buffer, &listen_addr);
		ip_recv = listen_addr.sin_addr.s_addr;

		if (ip_recv == ip_0) { //pkt from server
			memcpy((void *)&sv_addr, (void *)&listen_addr, sizeof(sv_addr));

			//according to current dst_idx to send ip
			if (dst_idx == 1) {
				set_addr(&cl_addr, ip_1, 5001);
				send_msg(connfd, buffer, &cl_addr);
			} else if (dst_idx == 2) {
				set_addr(&cl_addr, ip_2, 5001);
				send_msg(connfd, buffer, &cl_addr);
			}

		} else if (ip_recv == ip_1 || ip_recv == ip_2) { //pkt form client
			//send to server
			send_msg(connfd, buffer, &sv_addr);
		}
	}
}
