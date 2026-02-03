#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void run_port_slayer(char *port) {
  char comando[100];
  sprintf(comando, "lsof -t -i:%s", port);

  int status = system(comando);
  if (status == -1) {
    printf("Error fatal al ejecutar lsof: %s\n", strerror(errno));
    return;
  }

  int exit_code = WEXITSTATUS(status);

  if (exit_code != 0) {
    printf("No se encontraron procesos que usen el puerto %s\n", port);
    return;
  }

  printf("Proceso(s) encontrado(s). Liquidando...\n");
  char kill_command[100];
  sprintf(kill_command, "lsof -t -i:%s | xargs kill -9", port);

  int status_kill = system(kill_command);
  if (status_kill == -1) {
    printf("Error al ejecutar kill: %s\n", strerror(errno));
  } else if (WEXITSTATUS(status_kill) == 0) {
    printf("Proceso(s) eliminado(s) con el puerto %s\n", port);
  }
}
