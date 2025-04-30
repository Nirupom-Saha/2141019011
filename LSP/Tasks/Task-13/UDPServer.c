#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

int main(){
	int sockfd, len;
	struct sockaddr_in servaddr, clientaddr;
	char buff[1024];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	len = sizeof(clientaddr);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8080);

	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("UDP Server running on port: 8080\n");

	// Step 1: Receive the first message from client
	bzero(buff, sizeof(buff));
	recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, &len);
	printf("Client says: %s\n", buff);

	// Step 2: Send welcome message
	strcpy(buff, "Hello from Server");
	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&clientaddr, len);

	// Step 3: Loop for further communication
	while (1) {
		bzero(buff, sizeof(buff));
		recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, &len);
		printf("Client: %s", buff);

		if (strncmp(buff, "exit", 4) == 0) {
			printf("Session ended by client.\n");
			break;
		}

		printf("Server: ");
		fgets(buff, sizeof(buff), stdin);
		sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&clientaddr, len);
	}

	close(sockfd);
}

