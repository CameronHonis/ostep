#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void try_exec(const char *cmd) {
    if (strcmp(cmd, "execl") == 0) {
        execl("/bin/ls", ".", NULL);
    } else if (strcmp(cmd, "execlp") == 0) {
        execlp("ls", ".", NULL);
    } else if (strcmp(cmd, "execle") == 0) {
        char *envp[] = {"PATH=/usr/local/bin", NULL};
        execle("/bin/ls", ".", NULL, envp);
    } else if (strcmp(cmd, "execv") == 0) {
        char *args[] = {"/bin/ls", ".", NULL};
        execv("/bin/ls", args);
    } else if (strcmp(cmd, "execvp") == 0) {
        char *args[] = {"/bin/ls", ".", NULL};
        execvp("/bin/ls", args);
    } else if (strcmp(cmd, "execvpe") == 0) {
        char *envp[] = {"PATH=/usr/local/bin", NULL};
        char *args[] = {"ls", ".", NULL};
        execvpe("ls", args, envp);
    }
}

int main(int argc, char *argv[]) {
    const char *cmds[] = {"execl", "execlp", "execle",
                          "execv", "execvp", "execvpe"};
    int cmds_len = sizeof(cmds) / sizeof(cmds[0]);

    for (int i = 0; i < cmds_len; i++) {
        const char *cmd = cmds[i];
        pid_t pid = fork();
        assert(pid >= 0);
        if (pid > 0) {
            // parent
            wait(NULL);
        } else {
            // child
            printf("Executing %s...\n", cmd);
            try_exec(cmd);
        }
    }
}
