#include <stdio.h>
#include <fcntl.h>      //  open()
#include <unistd.h>     //  write(), read(), lseek(), close()
#include <string.h>     //  strlen()
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	int fd;
	char buffer[100];
	const char *text = "Hello, File Handling in C!";
	const char *filename = "data.txt";
    
	mode_t fmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // Setting file permission 

	fd = open(filename, O_RDWR | O_CREAT, fmode); //Opening the file and creating the file if it doesn't exist with read write permission
	
	if(fd == -1) {
		perror("Failed to open or create file");
		return 1;
	}

	if(write(fd, text, strlen(text)) == -1) { // performing write operation on the file 
		perror("Write failed");
		close(fd);
 		return 1;
	}

	if(lseek(fd, 0, SEEK_SET) == -1) { // moving the file pointer to the beginning
		perror("lseek failed");
 		close(fd);
		return 1;
	}

	ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1); // reading the content back
	if(bytesRead == -1) {
		perror("Error reading file");
		close(fd);
		return 1;
	}

	buffer[bytesRead] = '\0';   // terminating the buffer with Null-terminater 
	printf("File content: %s\n",buffer);

	close(fd);

	return 0;
}

