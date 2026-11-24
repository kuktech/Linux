#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP "220.149.128.101"
#define SERV_PORT 4465
#define P2P_port 4466

int main()
{
	int sockfd;
	struct sockaddr_in dest_addr;
	int rcv_byte1, rcv_byte2;
	char msg1[512];
	char msg2[512];

	char id[20];
	char pw[20];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd == -1)
	{
		perror("Client-socket() error lol!");
		exit(1);
	}
	else printf("Client-socket() sockfd is Ok...\n");

	dest_addr.sin_family = AF_INET;

	dest_addr.sin_port = htons(SERV_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(SERV_IP);

	memset(&(dest_addr.sin_zero),0,8);

	if(connect(sockfd, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr)) == -1){
		perror("Client-connect() error lol");
		exit(1);
	}
	else printf("Client-connect() is Ok...\n\n");



	if(recv(sockfd, msg1, sizeof(msg1), 0) > 0)
	{
		printf("%s\n", msg1);
	}
	else
	{
		perror("Transmit message error\n");
		exit(1);
	}	

    printf("ID : ");
	scanf("%s", id);
	send(sockfd, id, strlen(id) + 1, 0);
	printf("PW : ");
	scanf("%s", pw);
    send(sockfd, pw, strlen(pw) + 1, 0);

	rcv_byte2 = recv(sockfd, msg2, sizeof(msg2), 0);
	printf("%s\n", msg2);

    close(sockfd);
	return 0;
}
