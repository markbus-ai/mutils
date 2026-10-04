#include "../mutils.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

int mutils_strcasecmp(const char *s1, const char *s2) // implementacion de strcmp que ignora mayúsculas y minúsculas
{
    while (*s1 && tolower((unsigned char)*s1) == tolower((unsigned char)*s2))
    {
        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1) -
           tolower((unsigned char)*s2);
}


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

// Igual que run_cmd pero manda el stdout del hijo a out_path.
// Necesario para gzip/bzip2/xz, que solo saben escribir a stdout.
int run_cmd_redirect(const char *file, char *const argv[], const char *out_path) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        int fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            fprintf(stderr, "Error abriendo %s: %s\n", out_path, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            fprintf(stderr, "dup2 falló: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd);
        execvp(file, argv);
        fprintf(stderr, "Error executing %s: %s\n", file, strerror(errno));
        exit(EXIT_FAILURE);
    } else {
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
