#define _XOPEN_SOURCE 500
#include "../mutils.h"
#include <dirent.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PROCS 32

static int active_procs = 0;

int tiene_extension(const char *nombre, const char *ext) {
  const char *dot = strrchr(nombre, '.');
  if (!dot || dot == nombre)
    return 0;
  return strcmp(dot, ext) == 0;
}

int is_trash(const char *nombre, int es_directorio) {
  if (es_directorio) {
    if (strcmp(nombre, "__pycache__") == 0) return 1;
    if (strcmp(nombre, ".pytest_cache") == 0) return 1;
    if (strcmp(nombre, ".mypy_cache") == 0) return 1;
    if (strcmp(nombre, ".venv") == 0) return 1;
    if (strcmp(nombre, "build") == 0) return 1;
    if (strcmp(nombre, "dist") == 0) return 1;
    if (strcmp(nombre, "node_modules") == 0) return 1;
    if (strcmp(nombre, ".npm") == 0) return 1;
    if (strcmp(nombre, "CMakeFiles") == 0) return 1;
    if (strcmp(nombre, ".cache") == 0) return 1;
  } else {
    if (tiene_extension(nombre, ".pyc")) return 1;
    if (tiene_extension(nombre, ".pyo")) return 1;
    if (tiene_extension(nombre, ".log")) return 1;
    if (tiene_extension(nombre, ".o")) return 1;
    if (tiene_extension(nombre, ".out")) return 1;
    if (strcmp(nombre, ".DS_Store") == 0) return 1;
  }
  return 0;
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
  (void)sb; (void)typeflag; (void)ftwbuf;
  return remove(fpath);
}

void internal_rm_rf(const char *path) {
  nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

void scan_and_nuke(char *path) {
  DIR *d = opendir(path);
  struct dirent *entry;
  char ruta_completa[1024];

  if (!d) return;

  while ((entry = readdir(d)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, entry->d_name);

    struct stat st;
    if (lstat(ruta_completa, &st) == -1) continue;

    int es_dir = S_ISDIR(st.st_mode);

    if (is_trash(entry->d_name, es_dir)) {
      printf("Nukeando: %s\n", ruta_completa);

      if (es_dir) {
        // Control de concurrencia: si llegamos al límite, esperamos a que alguien termine
        while (active_procs >= MAX_PROCS) {
          wait(NULL);
          active_procs--;
        }

        pid_t pid = fork();
        if (pid == 0) {
          // Hijo: Borra y muere
          internal_rm_rf(ruta_completa);
          exit(0);
        } else if (pid > 0) {
          // Padre: Cuenta y sigue buscando
          active_procs++;
        } else {
          perror("fork failed");
        }
      } else {
        remove(ruta_completa);
      }
      continue; // No entramos en lo que borramos
    }

    if (es_dir) {
      scan_and_nuke(ruta_completa);
    }
  }
  closedir(d);
}

void run_nuke_dev(char *path) {
  active_procs = 0;
  
  // 1. Escanea y lanza procesos hijos
  scan_and_nuke(path);

  // 2. Espera a que terminen todos los hijos pendientes
  while (active_procs > 0) {
    wait(NULL);
    active_procs--;
  }
}