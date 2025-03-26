#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int x = open("x.txt", O_CREAT | O_TRUNC | O_WRONLY);
    assert(x >= 0);
    int c = fork();

    int bw;
    if (c < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (c == 0) {
        const char *msg = "this is from child\n";
        // write to x.txt in child process
        bw = write(x, msg, strlen(msg));
    } else {
        const char *msg = "this is from parent\n";
        bw = write(x, msg, strlen(msg));
    }

    if (bw < 0) {
        fprintf(stderr, "Write failed\n");
    }
}
