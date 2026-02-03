// main.c
#include "mutils.h"
#include <stdio.h>
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
  } else {
    printf("Comando '%s' no reconocido.\n", argv[1]);
  }

  return 0;
}
