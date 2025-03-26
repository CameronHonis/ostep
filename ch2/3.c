#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int c = fork();

    if (c < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (c == 0) {
        printf("hello\n");
    } else {
        sleep(1);
        printf("goodbye\n");
    }
}
