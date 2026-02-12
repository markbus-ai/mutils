#include "../mutils.h"
#include <stdio.h>
#include <stdlib.h>

void run_myip() {
  printf("Public IP: ");
  fflush(stdout);
  char *args[] = {"curl", "-s", "ifconfig.me", NULL};
  run_cmd("curl", args);
  printf("\n");
}
