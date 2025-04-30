#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
	int listenfd, connfd;
	struct sockaddr_in servaddr, clientaddr;
	socklen_t len;
	char buffer[1024];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(8080); 

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 5);

	printf("Server is listening on 127.0.0.1, Port: 8080\n");

	len = sizeof(clientaddr);
	connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);

	write(connfd, "Welcome to the Server!\n", 24);

	while (1) {
		bzero(buffer, sizeof(buffer));
		read(connfd, buffer, sizeof(buffer));
		printf("From client: %s", buffer);

 		if (strncmp("exit", buffer, 4) == 0) {
			printf("Client requested to exit. Closing connection.\n");
			break;
		}

		printf("Enter server reply: ");
 		bzero(buffer, sizeof(buffer));
		fgets(buffer, sizeof(buffer), stdin);
		write(connfd, buffer, strlen(buffer));
	}

	close(connfd);
	close(listenfd);

	return 0;
}

