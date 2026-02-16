
#include <leptonica/allheaders.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <tesseract/capi.h>
#include <unistd.h>

#define CHUNK_SIZE 4096

void *safe_realloc(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (!new_ptr) {
    perror("realloc falló");
    free(ptr);
    exit(EXIT_FAILURE);
  }
  return new_ptr;
}

void copiar_al_clipboard(const char *texto) {
  int pipe_clip[2];
  if (pipe(pipe_clip) == -1) {
    perror("pipe clipboard");
    return;
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork clipboard");
    return;
  }

  if (pid == 0) {
    if (dup2(pipe_clip[0], STDIN_FILENO) == -1) {
      perror("dup2 clip");
      exit(1);
    }
    close(pipe_clip[1]);
    close(pipe_clip[0]);
    char *args[] = {"wl-copy", NULL};
    execvp(args[0], args);
    perror("execvp wl-copy");
    exit(1);
  }

  close(pipe_clip[0]);
  if (write(pipe_clip[1], texto, strlen(texto)) == -1) {
    perror("write clipboard");
  }
  close(pipe_clip[1]);
  waitpid(pid, NULL, 0);
}

void run_ocr() {
  int pipe_slurp[2], pipe_grim[2];

  if (pipe(pipe_slurp) == -1) {
    perror("pipe slurp");
    return;
  }
  pid_t pid_slurp = fork();
  if (pid_slurp == -1) {
    perror("fork slurp");
    return;
  }

  if (pid_slurp == 0) {
    dup2(pipe_slurp[1], STDOUT_FILENO);
    close(pipe_slurp[0]);
    close(pipe_slurp[1]);
    char *args[] = {"slurp", NULL};
    execvp(args[0], args);
    exit(1);
  }
  close(pipe_slurp[1]);

  char region[256];
  ssize_t n = read(pipe_slurp[0], region, sizeof(region) - 1);
  if (n <= 0) {
    fprintf(stderr, "error: región inválida o cancelada\n");
    close(pipe_slurp[0]);
    return;
  }
  region[n - 1] = '\0';
  close(pipe_slurp[0]);
  waitpid(pid_slurp, NULL, 0);

  if (pipe(pipe_grim) == -1) {
    perror("pipe grim");
    return;
  }
  pid_t pid_grim = fork();
  if (pid_grim == -1) {
    perror("fork grim");
    return;
  }

  if (pid_grim == 0) {
    dup2(pipe_grim[1], STDOUT_FILENO);
    close(pipe_grim[0]);
    close(pipe_grim[1]);
    char *args[] = {"grim", "-g", region, "-", NULL};
    execvp(args[0], args);
    exit(1);
  }
  close(pipe_grim[1]);

  size_t capacity = CHUNK_SIZE, size = 0;
  unsigned char *buffer = malloc(capacity);
  if (!buffer) {
    perror("malloc");
    return;
  }

  ssize_t bytes;
  while ((bytes = read(pipe_grim[0], buffer + size, CHUNK_SIZE)) > 0) {
    size += bytes;
    if (size + CHUNK_SIZE > capacity) {
      capacity *= 2;
      buffer = safe_realloc(buffer, capacity);
    }
  }
  close(pipe_grim[0]);
  waitpid(pid_grim, NULL, 0);

  TessBaseAPI *handle = TessBaseAPICreate();
  if (TessBaseAPIInit3(handle, NULL, "spa") != 0) {
    fprintf(stderr, "error: falla en init tesseract\n");
    free(buffer);
    return;
  }

  PIX *img = pixReadMem(buffer, size);
  if (img) {
    TessBaseAPISetImage2(handle, img);
    char *text = TessBaseAPIGetUTF8Text(handle);
    if (text) {
      printf("--- ocr detectado ---\n%s", text);
      copiar_al_clipboard(text);
      TessDeleteText(text);
    }
    pixDestroy(&img);
  }

  TessBaseAPIEnd(handle);
  TessBaseAPIDelete(handle);
  free(buffer);
}
