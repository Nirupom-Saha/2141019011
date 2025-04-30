#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
	int sockfd;
	struct sockaddr_in servaddr;
	char buffer[1024];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	bzero(buffer, sizeof(buffer));
	read(sockfd, buffer, sizeof(buffer));
	printf("Server says: %s", buffer);

	while (1) {
		printf("Enter message: ");
		bzero(buffer, sizeof(buffer));
		fgets(buffer, sizeof(buffer), stdin);
		write(sockfd, buffer, strlen(buffer));

		if (strncmp("exit", buffer, 4) == 0) {
			printf("Disconnected from server.\n");
			break;
		}

		bzero(buffer, sizeof(buffer));
		read(sockfd, buffer, sizeof(buffer));
		printf("From server: %s", buffer);
	}

	close(sockfd);
	return 0;
}

