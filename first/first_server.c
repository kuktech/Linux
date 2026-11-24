#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERV_IP "220.149.128.101"
#define SERV_PORT 4465
#define BACKLOG 10

#define INIT_MSG "========================\nHello! I'm P2P File Sharing Server...\nPlease, LOG-IN!\n========================\n"
#define USER1_ID "David"
#define USER1_PW "1234"
#define USER2_ID "Lucy"
#define USER2_PW "5678"

#define FAIL_MSG "Log-in Fail : Incorrect password....\n\n"
int main()
{
	int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	unsigned int sin_size;
	
	int rcv_byte1, rcv_byte2;
	char id[20];
	char pw[20];
	char succ_msg[50];
	
	int val = 1;
	pid_t pid;
	
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("Server-socket() error lol!");
		exit(1);
	}
	else printf("Server-socket() sockfd is OK..\n");
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERV_PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), 0, 8);
	
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val))<0
	){
		perror("setsockopt");
		close(sockfd);
		return -1;
	}
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -
	1){
		perror("Server-bind() error lol!");
	}
	else printf("Server-bind() is Ok..\n");

	if(listen(sockfd, BACKLOG) == -1)
	{
		perror("listen() error lol!");
		exit(1);
	}
	else printf("listen () is OK...\n\n");

	sin_size = sizeof(struct sockaddr_in);

	while(1)
	{
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		printf("accept() is OK....\n\n");
		pid = fork();

		if(pid == -1)
		{
			perror("fork error\n");
			close(new_fd);
			exit(1);
		}
		else if(pid == 0)
		{
			send(new_fd, INIT_MSG, strlen(INIT_MSG) + 1, 0);

			rcv_byte1 = recv(new_fd, id, sizeof(id),0);
			rcv_byte2 = recv(new_fd, pw, sizeof(pw),0);

			printf("=======================\nUser Information\nID : %s\nPW : %s\n=======================\n", id,pw);

			if(((strcmp(id,USER1_ID)==0) && (strcmp(pw,USER1_PW)==0)) || (((strcmp(id,USER2_ID)==0) && (strcmp(pw,USER2_PW)==0))))
			{
				snprintf(succ_msg, sizeof(succ_msg), "Log-in success!! [%s] *^^*\n", id);
				printf("%s\n", succ_msg);
				send(new_fd,succ_msg, strlen(succ_msg)+1, 0);
			}
			else
			{
				printf(FAIL_MSG);
				send(new_fd,FAIL_MSG, strlen(FAIL_MSG) + 1, 0);
			}
			close(new_fd);
			close(sockfd);
			exit(0);
		}
		else
		{
			close(new_fd);
		}
	}
	
	close(sockfd);
	return 0;
}


