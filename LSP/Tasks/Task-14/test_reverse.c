// File: test_reverse.c
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#define SYS_reverse_string 548

int main() {
    char input[] = "HelloKernel";
    char output[100] = {0};

    long ret = syscall(SYS_reverse_string, input, output);
    if (ret == 0)
        printf("Reversed: %s\n", output);
    else
        perror("syscall failed");

    return 0;
}

