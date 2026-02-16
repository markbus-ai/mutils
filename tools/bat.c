#include "../mutils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void run_bat_threshold(int *max) {
  if (geteuid() != 0) {
    fprintf(stderr, "Error: Este comando requiere privilegios de root.\n");
    fprintf(stderr, "Por favor, ejecutalo con sudo: sudo mutils bat %d\n", *max);
    return;
  }

  const char *filepath = "/etc/udev/rules.d/99-battery-threshold.rules";
  FILE *f = fopen(filepath, "w");
  if (!f) {
    fprintf(stderr, "Error al abrir %s para escritura: %s\n", filepath, strerror(errno));
    return;
  }

  fprintf(f, "ACTION==\"add|change\", SUBSYSTEM==\"power_supply\", "
             "KERNEL==\"BAT0\", ATTR{charge_control_end_threshold}=\"%d\"\n",
          *max);
  fclose(f);

  char *args_reload[] = {"udevadm", "control", "--reload-rules", NULL};
  if (run_cmd("udevadm", args_reload) != 0) {
    fprintf(stderr, "Error al recargar reglas udev.\n");
    return;
  }

  char *args_trigger[] = {"udevadm", "trigger", NULL};
  if (run_cmd("udevadm", args_trigger) != 0) {
    fprintf(stderr, "Error al disparar trigger udev.\n");
    return;
  }

  printf("Udevadm exitoso\n");
}

void run_bat_health() {

  FILE *f_now, *f_orig;
  char buf_now[32], buf_orig[32];

  f_now = fopen("/sys/class/power_supply/BAT0/charge_full", "r");
  f_orig = fopen("/sys/class/power_supply/BAT0/charge_full_design", "r");

  if (!f_now || !f_orig) {

    if (f_now)
      fclose(f_now);
    if (f_orig)
      fclose(f_orig);
    printf("-1.0");
    return;
  }

  fgets(buf_now, sizeof(buf_now), f_now);
  fgets(buf_orig, sizeof(buf_orig), f_orig);

  fclose(f_now);
  fclose(f_orig);

  long current_max = strtol(buf_now, NULL, 10);
  long design_max = strtol(buf_orig, NULL, 10);

  if (design_max == 0)
    printf("0.0");

  printf("Salud: %.2f%%\n", ((float)current_max / (float)design_max) * 100.0f);
}
