#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "udp.h"

struct sockaddr_in claddr;

void *keep_listen(void *fd);

int main(int argc, char* argv[])
{
	if (argc != 2) {
		puts("Usage: ./server <port>");
		exit(EXIT_FAILURE);
	}

	int svfd;
	int seq_num;
	char msg[BUFFSIZE + 1];
	pthread_t t_keep_listen;

	//get server socket fd
	svfd = sv_udp(atoi(argv[1]));
	
	//recv msg and show recv msg from client
	listen_fd(svfd);

	pthread_create(&t_keep_listen, NULL, keep_listen, (void *)&svfd);

	//send sequence number to client
	seq_num = 0;
	while (1) {
		snprintf(msg, sizeof(msg), "%d", seq_num++);

		send_msg(svfd, msg, &claddr);

		sleep(1);
	}
}

void *keep_listen(void *fd)
{
	int svfd = *(int *)fd;
	while (1) {
		listen_fd(svfd);
	}
}
