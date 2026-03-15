#include "mutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help() {
  printf("MUtils - Herramientas modulares para desarrolladores Linux\n\n");
  printf("Uso: mutils <comando> [argumentos]\n\n");
  printf("Comandos:\n");
  printf("  port <puerto>       Mata procesos que ocupan el puerto especificado.\n");
  printf("  bat <porcentaje>    Establece el umbral de carga de la batería (requiere udev).\n");
  printf("  bat health          Muestra la salud y estado de la batería.\n");
  printf("  nuke <path>         Limpia recursivamente archivos basura (node_modules, build, etc.).\n");
  printf("  env                 Valida claves en .env contra .env.example.\n");
  printf("  ip                  Muestra la dirección IP pública.\n");
  printf("  ping                Diagnóstico de red (DNS, latencia, jitter, pérdida de paquetes).\n");
  printf("  x <archivo>         Extractor inteligente para varios formatos (.tar, .zip, .rar, etc.).\n");
  printf("  ocr                 Extracción de texto de la pantalla usando OCR (Wayland).\n");
  printf("  --help, -h          Muestra este mensaje de ayuda.\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
    print_help();
    return 0;
  }

  if (strcmp(argv[1], "port") == 0) {
    if (argc < 3 || strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0) {
      printf("Uso: mutils port <puerto>\n");
      return 0;
    }
    run_port_slayer(argv[2]);
  } else if (strcmp(argv[1], "bat") == 0) {
    if (argc < 3 || strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0) {
      printf("Uso: mutils bat <porcentaje|health>\n");
      return 0;
    }
    if (strcmp(argv[2], "health") == 0) {
      run_bat_health();
      return 0;
    }
    int max = atoi(argv[2]);
    run_bat_threshold(&max);
  } else if (strcmp(argv[1], "nuke") == 0) {
    if (argc < 3 || strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0) {
      printf("Uso: mutils nuke <path>\n");
      return 0;
    }

    run_nuke_dev(argv[2]);
  } else if (strcmp(argv[1], "env") == 0) {
    if (argc >= 3 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0)) {
      printf("Uso: mutils env\n");
      return 0;
    }
    run_env_check();
  } else if (strcmp(argv[1], "ip") == 0) {
    if (argc >= 3 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0)) {
      printf("Uso: mutils ip\n");
      return 0;
    }
    run_myip();
  } else if (strcmp(argv[1], "ping") == 0) {
    if (argc >= 3 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0)) {
      printf("Uso: mutils ping\n");
      return 0;
    }
    run_ping_check();
  } else if (strcmp(argv[1], "x") == 0) {
    if (argc < 3 || strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0) {
      printf("Uso: mutils x <archivo>\n");
      return 0;
    }
    run_extract(argv[2]);
  } else if (strcmp(argv[1], "ocr") == 0) {
    if (argc >= 3 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0)) {
      printf("Uso: mutils ocr\n");
      return 0;
    }
    run_ocr();
  } else {
    printf("Error: Comando '%s' no reconocido.\n", argv[1]);
    printf("Escriba 'mutils --help' para ver los comandos disponibles.\n");
    return 1;
  }
  return 0;
}
