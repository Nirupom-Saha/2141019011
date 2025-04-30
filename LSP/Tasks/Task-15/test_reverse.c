#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

#define SYS_reverse_string 548       
#define MAX_LEN 256

int main() {
    char *input = malloc(MAX_LEN);
    char *output = malloc(MAX_LEN);

    if (!input || !output) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    printf("Enter a string to reverse (max %d characters): ", MAX_LEN - 1);
    if (fgets(input, MAX_LEN, stdin) == NULL) {
        perror("Failed to read input");
        free(input);
        free(output);
        return EXIT_FAILURE;
    }

    // Remove trailing newline
    size_t len = strlen(input);
    if (input[len - 1] == '\n') input[len - 1] = '\0';

    long status = syscall(SYS_reverse_string, input, output);
    if (status == 0) {
        printf("Reversed string: %s\n", output);
    } else {
        perror("System call failed");
    }

    free(input);
    free(output);
    return 0;
}

