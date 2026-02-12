#include "../mutils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int tiene_extension(const char *nombre, const char *ext) {
  const char *dot = strrchr(nombre, '.'); // Busca el último punto
  if (!dot || dot == nombre)
    return 0;
  return strcmp(dot, ext) == 0;
}

int is_trash(const char *nombre, int es_directorio) {
  if (es_directorio) {
    // Python
    if (strcmp(nombre, "__pycache__") == 0)
      return 1;
    if (strcmp(nombre, ".pytest_cache") == 0)
      return 1;
    if (strcmp(nombre, ".mypy_cache") == 0)
      return 1;
    if (strcmp(nombre, ".venv") == 0)
      return 1; // Opcional: entornos virtuales
    if (strcmp(nombre, "build") == 0)
      return 1; // Python setup.py build
    if (strcmp(nombre, "dist") == 0)
      return 1; // Python dists

    // Node / JS
    if (strcmp(nombre, "node_modules") == 0)
      return 1;
    if (strcmp(nombre, ".npm") == 0)
      return 1;

    // C / C++ / General
    if (strcmp(nombre, "CMakeFiles") == 0)
      return 1; // CMake
    if (strcmp(nombre, ".cache") == 0)
      return 1;
  } else {
    // Archivos sueltos
    if (tiene_extension(nombre, ".pyc"))
      return 1; // Python compilado
    if (tiene_extension(nombre, ".pyo"))
      return 1; // Python optimizado
    if (tiene_extension(nombre, ".log"))
      return 1; // Logs genéricos
    if (tiene_extension(nombre, ".o"))
      return 1; // Objetos de C
    if (tiene_extension(nombre, ".out"))
      return 1; // Binarios a.out
    if (strcmp(nombre, ".DS_Store") == 0)
      return 1; // Basura de Mac
  }
  return 0;
}

void run_nuke_dev(char *path) {
  DIR *d = opendir(path);
  struct dirent *entry;
  char ruta_completa[1024]; // Buffer para armar el path del hijo

  if (!d)
    return;

  while ((entry = readdir(d)) != NULL) {
    // 1. Ignorar . y .. para no romper el universo
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // Armamos la ruta completa: path actual + / + nombre archivo
    snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path,
             entry->d_name);

    int es_dir = (entry->d_type == DT_DIR);

    // 2. Chequear si es basura conocida
    if (is_trash(entry->d_name, es_dir)) {
      printf("Nukeando: %s\n", ruta_completa);

      // Usamos rm -rf para asegurar borrado recursivo de carpetas basura
      char *args[] = {"rm", "-rf", ruta_completa, NULL};
      run_cmd("rm", args);

      // Si borramos la carpeta, NO intentamos entrar (recursesión) en ella
      continue;
    }

    // 3. Si es un directorio normal (no basura), entramos recursivamente
    if (es_dir) {
      run_nuke_dev(ruta_completa);
    }
  }

  closedir(d);
}
