#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

int main(){
	int sockfd, len;
	struct sockaddr_in servaddr;
	char buff[1024];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	len = sizeof(servaddr);

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

	// Step 1: Send initial message
	strcpy(buff, "Hello from Client");
	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&servaddr, len);

	// Step 2: Receive welcome message
	bzero(buff, sizeof(buff));
	recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, &len);
	printf("Server says: %s\n", buff);

	// Step 3: Loop for further communication
	while (1) {
		printf("Client: ");
		fgets(buff, sizeof(buff), stdin);
		sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&servaddr, len);

		if (strncmp(buff, "exit", 4) == 0) {
			printf("Exiting chat.\n");
			break;
		}

		bzero(buff, sizeof(buff));
		recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, &len);
		printf("Server: %s", buff);
	}

	close(sockfd);
}

