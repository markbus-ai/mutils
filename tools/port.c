#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void run_port_slayer(char *port) {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe");
    return;
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return;
  }

  if (pid == 0) {
    close(pipefd[0]);
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    close(pipefd[1]);

    char arg_port[32];
    snprintf(arg_port, sizeof(arg_port), "-i:%s", port);
    char *args[] = {"lsof", "-t", arg_port, NULL};
    
    execvp("lsof", args);
    perror("execvp lsof");
    exit(EXIT_FAILURE);
  } else {
    close(pipefd[1]);

    FILE *stream = fdopen(pipefd[0], "r");
    if (!stream) {
      perror("fdopen");
      close(pipefd[0]);
      return;
    }

    char line[32];
    int found = 0;
    while (fgets(line, sizeof(line), stream)) {
      found = 1;
      pid_t target_pid = (pid_t)strtol(line, NULL, 10);
      if (target_pid > 0) {
        printf("Matando proceso PID: %d\n", target_pid);
        if (kill(target_pid, SIGKILL) == -1) {
            perror("kill");
        }
      }
    }
    fclose(stream);
    
    int status;
    waitpid(pid, &status, 0);
    
    if (!found) {
        printf("No se encontraron procesos en el puerto %s\n", port);
    } else {
        printf("Limpieza completada.\n");
    }
  }
}
