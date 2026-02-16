#include "../mutils.h"
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

  const char *tool = NULL;
  char *prog = NULL;
  char *argv[5];

  if (ends_with(filename, ".tar.gz") || ends_with(filename, ".tgz")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-xzvf"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "tar (gzip)";
  } else if (ends_with(filename, ".tar.bz2") || ends_with(filename, ".tbz2")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-xjvf"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "tar (bzip2)";
  } else if (ends_with(filename, ".tar.xz") || ends_with(filename, ".txz")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-xJvf"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "tar (xz)";
  } else if (ends_with(filename, ".tar")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-xvf"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "tar";
  }

  else if (ends_with(filename, ".zip") || ends_with(filename, ".jar") ||
           ends_with(filename, ".war")) {
    prog = "unzip";
    argv[0] = "unzip"; argv[1] = (char *)filename; argv[2] = NULL;
    tool = "unzip";
  } else if (ends_with(filename, ".rar")) {
    prog = "unrar";
    argv[0] = "unrar"; argv[1] = "x"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "unrar";
  } else if (ends_with(filename, ".7z")) {
    prog = "7z";
    argv[0] = "7z"; argv[1] = "x"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "7zip";
  }

  else if (ends_with(filename, ".gz")) {
    prog = "gunzip";
    argv[0] = "gunzip"; argv[1] = "-k"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "gunzip";
  } else if (ends_with(filename, ".bz2")) {
    prog = "bunzip2";
    argv[0] = "bunzip2"; argv[1] = "-k"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "bunzip2";
  } else if (ends_with(filename, ".xz")) {
    prog = "unxz";
    argv[0] = "unxz"; argv[1] = "-k"; argv[2] = (char *)filename; argv[3] = NULL;
    tool = "unxz";
  } else if (ends_with(filename, ".Z")) {
    prog = "uncompress";
    argv[0] = "uncompress"; argv[1] = (char *)filename; argv[2] = NULL;
    tool = "uncompress";
  }

  if (prog) {
    printf("Extrayendo [%s] usando %s...\n", filename, tool);
    int status = run_cmd(prog, argv);

    if (status != 0) {
      printf("❌ Falló la extracción. Revisá el mensaje de error de arriba.\n");
      printf("   (Asegurate de que '%s' esté instalado y el archivo no esté corrupto)\n", tool);
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
