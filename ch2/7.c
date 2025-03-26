#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
/*#include <stdlib.h>*/
/*#include <string.h>*/
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid > 0) {
        // parent
    } else {
        // child
        close(STDOUT_FILENO);
        printf("hello world\n");
    }
}
