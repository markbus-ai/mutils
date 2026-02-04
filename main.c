// main.c
#include "mutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Verificamos si el usuario pasó al menos un comando
  if (argc < 2) {
    printf("Uso: mutils <comando> [argumentos]\n");
    return 1;
  }

  // Lógica del despachador
  if (strcmp(argv[1], "port") == 0) {
    if (argc < 3) {
      printf("Error: Falta especificar el puerto.\n");
      return 1;
    }
    run_port_slayer(argv[2]);
  }
  if (strcmp(argv[1], "bat") == 0) {
    if (strcmp(argv[2], "health") == 0) {
      run_bat_health();
      return 0;
    }
    if (argc < 3) {
      printf("Error: Falta especificar el porcentaje.\n");
      return 1;
    }
    int max = atoi(argv[2]);
    run_bat_threshold(&max);
  }
  if (strcmp(argv[1], "nuke") == 0) {
    if (argc < 3) {
      printf("Error: Falta especificar el path.\n");
      return 1;
    }

    run_nuke_dev(argv[2]);
  }
  if (strcmp(argv[1], "env") == 0) {
    run_env_check();
  }
  if (strcmp(argv[1], "ip") == 0) {
    run_myip();
    return 0;
  }
  if (strcmp(argv[1], "ping") == 0) {

    run_ping_check();

    return 0;
  }
}
