#include <assert.h>
/*#include <fcntl.h>*/
#include <stdio.h>
#include <stdlib.h>
/*#include <string.h>*/
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int x = 100;
    int c = fork();

    if (c < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (c == 0) {
        printf("Child: x = %d\n", x);
    } else {
        int w = wait(NULL);
        printf("Parent: x = %d\n", x);
        assert(w >= 0);
    }
}
