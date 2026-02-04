#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper para comparar el final del string (case insensitive sería ideal,
// pero en Linux las extensiones suelen ser minúsculas. Lo mantenemos simple).
int ends_with(const char *str, const char *suffix) {
  if (!str || !suffix)
    return 0;
  size_t len_str = strlen(str);
  size_t len_suffix = strlen(suffix);
  if (len_suffix > len_str)
    return 0;
  return strcmp(str + len_str - len_suffix, suffix) == 0;
}

void run_extract(const char *filename) {
  if (!filename) {
    printf("Falta el archivo a descomprimir.\n");
    return;
  }

  char cmd[1024];
  const char *tool = NULL;

  // --- GRUPO TAR (Prioridad Alta: extensiones compuestas) ---
  if (ends_with(filename, ".tar.gz") || ends_with(filename, ".tgz")) {
    snprintf(cmd, sizeof(cmd), "tar -xzvf \"%s\"", filename);
    tool = "tar (gzip)";
  } else if (ends_with(filename, ".tar.bz2") || ends_with(filename, ".tbz2")) {
    snprintf(cmd, sizeof(cmd), "tar -xjvf \"%s\"", filename);
    tool = "tar (bzip2)";
  } else if (ends_with(filename, ".tar.xz") || ends_with(filename, ".txz")) {
    snprintf(cmd, sizeof(cmd), "tar -xJvf \"%s\"", filename);
    tool = "tar (xz)";
  } else if (ends_with(filename, ".tar")) {
    snprintf(cmd, sizeof(cmd), "tar -xvf \"%s\"", filename);
    tool = "tar";
  }

  // --- GRUPO COMPRIMIDOS COMUNES ---
  else if (ends_with(filename, ".zip") || ends_with(filename, ".jar") ||
           ends_with(filename, ".war")) {
    // .jar y .war son zips glorificados
    snprintf(cmd, sizeof(cmd), "unzip \"%s\"", filename);
    tool = "unzip";
  } else if (ends_with(filename, ".rar")) {
    // Requiere 'unrar' instalado (sudo apt install unrar)
    snprintf(cmd, sizeof(cmd), "unrar x \"%s\"", filename);
    tool = "unrar";
  } else if (ends_with(filename, ".7z")) {
    // Requiere 'p7zip-full'
    snprintf(cmd, sizeof(cmd), "7z x \"%s\"", filename);
    tool = "7zip";
  }

  // --- GRUPO ARCHIVOS ÚNICOS (No son carpetas empaquetadas) ---
  else if (ends_with(filename, ".gz")) {
    snprintf(cmd, sizeof(cmd), "gunzip -k \"%s\"",
             filename); // -k: keep original
    tool = "gunzip";
  } else if (ends_with(filename, ".bz2")) {
    snprintf(cmd, sizeof(cmd), "bunzip2 -k \"%s\"", filename);
    tool = "bunzip2";
  } else if (ends_with(filename, ".xz")) {
    snprintf(cmd, sizeof(cmd), "unxz -k \"%s\"", filename);
    tool = "unxz";
  } else if (ends_with(filename, ".Z")) {
    snprintf(cmd, sizeof(cmd), "uncompress \"%s\"", filename);
    tool = "uncompress";
  }

  // --- EJECUCIÓN ---
  if (tool) {
    printf("Extrayendo [%s] usando %s...\n", filename, tool);
    int status = system(cmd);

    if (status != 0) {
      printf("Falló la extracción. ¿Tenés instalado '%s'?\n", tool);
      // Hint específico para herramientas no standard
      if (strcmp(tool, "unrar") == 0)
        printf("   Tip: sudo pacman -S unrar\n");
      if (strcmp(tool, "7zip") == 0)
        printf("    Tip:  sudo pacman -S p7zip-full\n");
    } else {
      printf("✅ Listo.\n");
    }
  } else {
    printf("Formato no soportado automáticamente.\n");
    printf("(Intentá descomprimirlo a mano o revisá la extensión)\n");
  }
}
