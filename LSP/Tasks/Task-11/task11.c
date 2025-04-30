#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main() {

	pid_t pid;
	int shmid;
	char *shmem;  
	key_t key = ftok(".", 100); 

	shmid = shmget(key, 1024, 0666 | IPC_CREAT); // it will create or get shared memory
	if (shmid == -1) {
		perror("shmget");
		exit(1);
	}

	shmem = (char *)shmat(shmid, NULL, 0); // it will attach shared memory to process
	if (shmem == (char *)-1) {
		perror("shmat");
		exit(1);
	}

	pid = fork();

	if (pid > 0) {                       // parent process to write in shared memory
	
		strcpy(shmem, "Hello from parent via shared memory!"); 
		printf("Parent: Message written to shared memory.\n");
		wait(NULL); 

		shmdt(shmem);
		shmctl(shmid, IPC_RMID, NULL);
		
	}
	else if (pid == 0) {                // child process to read 
		sleep(1); 
		printf("Child: Message read from shared memory: %s\n", shmem);
		shmdt(shmem);
		exit(0);
	}
	else {
		perror("Error in fork");
		exit(1);
	}

	return 0;
}

