#include "../mutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper para comparar si un string termina con un sufijo usando una implementación case-insensitive
static int ends_with(const char *str, const char *suffix) {
  if (!str || !suffix)
    return 0;
  size_t len_str = strlen(str);
  size_t len_suffix = strlen(suffix);
  if (len_suffix > len_str)
    return 0;
  return mutils_strcasecmp(str + len_str - len_suffix, suffix) == 0;
}

void run_compress(const char *filename, const char *output) {
  if (!filename) {
    printf("Falta el archivo a comprimir.\n");
    return;
  }
  char default_output[4096];
  if (!output) {
    snprintf(default_output, sizeof(default_output), "%s.tar.gz", filename);
    output = default_output;
  }

  const char *tool = NULL;
  char *prog = NULL;
  char *argv[5];
  int redirect = 0; // gzip/bzip2/xz escriben a stdout, hay que redirigir a output
  if (ends_with(output, ".tar.gz") || ends_with(output, ".tgz")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-czvf";
    argv[2] = (char *)output;
    argv[3] = (char *)filename;
    argv[4] = NULL;
    tool = "tar (gzip)";
} else if (ends_with(output, ".tar.bz2") || ends_with(output, ".tbz2")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-cjvf";
    argv[2] = (char *)output;
    argv[3] = (char *)filename;
    argv[4] = NULL;
    tool = "tar (bzip2)";
} else if (ends_with(output, ".tar.xz") || ends_with(output, ".txz")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-cJvf";
    argv[2] = (char *)output;
    argv[3] = (char *)filename;
    argv[4] = NULL;
    tool = "tar (xz)";
} else if (ends_with(output, ".tar")) {
    prog = "tar";
    argv[0] = "tar"; argv[1] = "-cvf";
    argv[2] = (char *)output;
    argv[3] = (char *)filename;
    argv[4] = NULL;
    tool = "tar";
}

else if (ends_with(output, ".zip") ||
         ends_with(output, ".jar") ||
         ends_with(output, ".war")) {
    prog = "zip";
    argv[0] = "zip";
    argv[1] = "-r";
    argv[2] = (char *)output;
    argv[3] = (char *)filename;
    argv[4] = NULL;
    tool = "zip";
}

else if (ends_with(output, ".rar")) {
    prog = "rar";
    argv[0] = "rar";
    argv[1] = "a";
    argv[2] = (char *)output;
    argv[3] = (char *)filename;
    argv[4] = NULL;
    tool = "rar";
}

else if (ends_with(output, ".7z")) {
    prog = "7z";
    argv[0] = "7z";
    argv[1] = "a";
    argv[2] = (char *)output;
    argv[3] = (char *)filename;
    argv[4] = NULL;
    tool = "7zip";
}

else if (ends_with(output, ".gz")) {
    prog = "gzip";
    argv[0] = "gzip";
    argv[1] = "-c";
    argv[2] = (char *)filename;
    argv[3] = NULL;
    tool = "gzip";
    redirect = 1;
}

else if (ends_with(output, ".bz2")) {
    prog = "bzip2";
    argv[0] = "bzip2";
    argv[1] = "-c";
    argv[2] = (char *)filename;
    argv[3] = NULL;
    tool = "bzip2";
    redirect = 1;
}

else if (ends_with(output, ".xz")) {
    prog = "xz";
    argv[0] = "xz";
    argv[1] = "-c";
    argv[2] = (char *)filename;
    argv[3] = NULL;
    tool = "xz";
    redirect = 1;
}

else if (ends_with(output, ".Z")) {
    prog = "compress";
    argv[0] = "compress";
    argv[1] = (char *)filename;
    argv[2] = NULL;
    tool = "compress";
}
if (prog) {
    printf("Comprimiendo [%s] → [%s] usando %s...\n", filename, output, tool);
    int status = redirect ? run_cmd_redirect(prog, argv, output) : run_cmd(prog, argv);

    if (status != 0) {
      printf("❌ Falló la compresion. Revisá el mensaje de error de arriba.\n");
      printf("   (Asegurate de que '%s' esté instalado y el archivo no esté corrupto)\n", tool);
      if (mutils_strcasecmp(tool, "unrar") == 0)
        printf("   Tip: sudo pacman -S unrar\n");
      if (mutils_strcasecmp(tool, "7zip") == 0)
        printf("    Tip:  sudo pacman -S p7zip-full\n");
    } else {
      printf("✅ Listo.\n");
    }
  } else {
    printf("Formato no soportado automáticamente.\n");
    printf("(Intentá comprimirlo a mano o revisá la extensión)\n");
  }
}
