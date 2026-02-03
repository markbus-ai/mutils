#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void run_bat_threshold(int *max) {
  char comando[256]; // Un tamaño generoso para un comando largo
  sprintf(comando,
          "echo 'ACTION==\"add|change\", SUBSYSTEM==\"power_supply\", "
          "KERNEL==\"BAT0\", ATTR{charge_control_end_threshold}=\"%d\"' | sudo "
          "tee /etc/udev/rules.d/99-battery-threshold.rules",
          *max);

  int status = system(comando);
  if (status == -1) {
    printf("Error fatal al ejecutar udev seter: %s\n", strerror(errno));
    return;
  } else if (WEXITSTATUS(status) != 0) {
    printf("Error al ejecutar udevadm: %s\n", strerror(errno));
    return;
  } else {
    int status_reload = system("sudo udevadm control --reload-rules");
    if (status_reload == -1) {
      printf("Error fatal al ejecutar udevadm: %s\n", strerror(errno));
      return;
    } else if (WEXITSTATUS(status_reload) != 0) {
      printf("Error al ejecutar udevadm: %s\n", strerror(errno));
      return;
    }

    int status_trigger = system("sudo udevadm trigger");
    if (status_trigger == -1) {
      printf("Error fatal al ejecutar udevadm: %s\n", strerror(errno));
      return;
    } else if (WEXITSTATUS(status_trigger) != 0) {
      printf("Error al ejecutar udevadm: %s\n", strerror(errno));
      return;
    }

    printf("Udevadm exitoso\n");
  }
}

void run_bat_health() {

  FILE *f_now, *f_orig;
  char buf_now[32], buf_orig[32];

  // Abrimos ambos archivos
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

  // Usamos strtol correctamente: string, endptr (NULL si no validamos), base 10
  long current_max = strtol(buf_now, NULL, 10);
  long design_max = strtol(buf_orig, NULL, 10);

  if (design_max == 0)
    printf("0.0");

  // Salud = (capacidad_actual / capacidad_original) * 100
  printf("Salud: %.2f%%\n", ((float)current_max / (float)design_max) * 100.0f);
}
