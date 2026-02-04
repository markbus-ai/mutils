#include <stdio.h>
#include <stdlib.h>
void run_myip() {
  printf("Public IP: ");
  fflush(stdout);
  system("curl ifconfig.me");
  printf("\n");
}
