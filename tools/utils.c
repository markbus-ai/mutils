#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int run_cmd(const char *file, char *const argv[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // Child process
        execvp(file, argv);
        // If execvp returns, it must have failed
        fprintf(stderr, "Error executing %s: %s\n", file, strerror(errno));
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return -1;
        }

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}
