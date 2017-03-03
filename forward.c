/* Sample UDP server */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char**argv)
{
   int sockfdWlan0, sockfdEth, n;
   int addr_len = sizeof(struct sockaddr_in);
   struct sockaddr_in servaddrWlan0, servaddrEth ,addr;
   socklen_t len;
   char mesg[100000];
   struct addrinfo hints, *res;

   sockfdWlan0=socket(AF_INET,SOCK_DGRAM,0);

   memset(&servaddrWlan0, 0, sizeof(servaddrWlan0));
   servaddrWlan0.sin_family = AF_INET;
   servaddrWlan0.sin_port=htons(8000);
   
   inet_pton(AF_INET, "140.113.207.236", &(servaddrWlan0.sin_addr));
   
   bind(sockfdWlan0,(struct sockaddr *)&servaddrWlan0,sizeof(servaddrWlan0));
   
   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_DGRAM;
   
 //  getaddrinfo("140.113.207.242", "6000", &hints, &res);
   getaddrinfo("140.113.207.231", "6000", &hints, &res);
   connect(sockfdWlan0, res->ai_addr, res->ai_addrlen);
	 printf("errno: %d\n", errno);
   perror("connect");
   

   sockfdEth=socket(AF_INET,SOCK_DGRAM,0);

   memset(&servaddrEth, 0, sizeof(servaddrEth));
   servaddrEth.sin_family = AF_INET;
   servaddrEth.sin_port=htons(7000);
   
   inet_pton(AF_INET, "140.113.207.236", &(servaddrEth.sin_addr));
   
   bind(sockfdEth,(struct sockaddr *)&servaddrEth,sizeof(servaddrEth));
   perror("bind");
   
   /*memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_DGRAM;
   
   getaddrinfo("192.168.6.128", "8000", &hints, &res);
   connect(sockfdEth, res->ai_addr, res->ai_addrlen);
   printf("errno: %d\n", errno);
   perror("connect");
   */
	 
   for (;;)
   {
   		n = recvfrom(sockfdEth, mesg, sizeof(mesg), 0, (struct sockaddr *) &addr, &addr_len);
   		send(sockfdWlan0, mesg, n, 0);
			printf("length: %d\n", n); 
     
   }
}
