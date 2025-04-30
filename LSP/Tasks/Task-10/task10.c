#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	int status;

	pid = fork();

	if (pid < 0) {
		perror("fork failed");
		return 1;
	}
	
	else if (pid == 0) {
        
		printf("Child process (PID: %d) executing 'ls -l'\n", getpid());
		char *args[] = {"ls", "-l", NULL};
		execvp(args[0], args);

		perror("execvp failed");
		exit(1);
	}
	
	else {

		printf("Parent process (PID: %d) waiting for child to finish..\n", getpid());
		wait(&status);
		printf("Child process terminated with status %d\n", WEXITSTATUS(status));
	}

	return 0;
}

