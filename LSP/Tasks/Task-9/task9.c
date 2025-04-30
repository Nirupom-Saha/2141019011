#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
	DIR *dir;
	struct dirent *entry;
	char currWorkingDir[1024];

	if (getcwd(currWorkingDir, sizeof(currWorkingDir)) == NULL) {  //getting and listing all the files and directories present in the current directory
		perror("Couldn't get current directory");
        	return 1;
    }

	printf("Current working directory: %s\n", currWorkingDir);
	printf("Files and directories:\n");

	dir = opendir(currWorkingDir);
	if(dir == NULL){
		perror("Could not open the directory");
		return 1;
	}

	while((entry = readdir(dir)) != NULL){
		if(entry->d_name[0] != '.')
			printf(" - %s\n", entry->d_name);
	}
	closedir(dir);


	if(chdir("/tmp") != 0) {                             //Changing directory to tmp
		perror("Failed to change directory to /tmp");
		return 1;
	}
	
	char tmpDir[1024];
	getcwd(tmpDir, sizeof(tmpDir));
	printf("\nChanged directory to: %s\n", tmpDir);

	int status = mkdir("testdir",0755);
	if(status == 0){
		printf("Directory 'testdir' successfully created.\n");
	}else{
		if(errno == EEXIST){
			printf("Directory already exists.\n");
		}else{
			perror("Failed to make directory.");
			return 1;
		}
	}

    return 0;
}

