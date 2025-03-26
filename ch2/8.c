#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pids[2];
    int pipe_fd[2];
    
    // Create a pipe
    assert(pipe(pipe_fd) == 0);
    
    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        assert(pid >= 0);
        
        if (pid > 0) {
            // Parent process
            pids[i] = pid;
        } else if (pid == 0) {
            // Child process
            if (i == 0) {
                // First child: write to the pipe
                close(pipe_fd[0]);  // Close unused read end
                dup2(pipe_fd[1], STDOUT_FILENO);  // Redirect stdout to pipe
                close(pipe_fd[1]);  // Close original pipe fd after duplication
                
                // Execute first command
                execlp("ls", "ls", "-l", NULL);
                perror("execlp (ls)");
                exit(1);
            } else if (i == 1) {
                // Second child: read from the pipe
                close(pipe_fd[1]);  // Close unused write end
                dup2(pipe_fd[0], STDIN_FILENO);  // Redirect stdin from pipe
                close(pipe_fd[0]);  // Close original pipe fd after duplication
                
                // Execute second command
                execlp("grep", "grep", "^d", NULL);  // Example: grep for directories
                perror("execlp (grep)");
                exit(1);
            }
            exit(0);  // Should never reach here
        }
    }
    
    // Parent process
    // Close both ends of the pipe in the parent
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    
    // Wait for both children to finish
    for (int i = 0; i < 2; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}
