#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LEN 64
#define MAX_LINE 1024

typedef struct KeyNode {
  char *key;
  struct KeyNode *next;
} KeyNode;

static KeyNode *head = NULL;

void add_key(const char *new_key) {
  KeyNode *node = malloc(sizeof(KeyNode));
  if (!node)
    exit(1);

  node->key = strdup(new_key);
  node->next = NULL;

  if (!head) {
    head = node;
  } else {
    KeyNode *curr = head;
    while (curr->next)
      curr = curr->next;
    curr->next = node;
  }
}

int has_key(const char *target) {
  KeyNode *curr = head;
  while (curr) {
    if (strcmp(curr->key, target) == 0)
      return 1;
    curr = curr->next;
  }
  return 0;
}

void free_list() {
  KeyNode *curr = head;
  while (curr) {
    KeyNode *temp = curr;
    curr = curr->next;
    free(temp->key);
    free(temp);
  }
  head = NULL;
}

int parse_key(char *line, char *buffer) {
  char *p = line;
  while (isspace((unsigned char)*p))
    p++;

  if (*p == '#' || *p == '\0' || *p == '\n')
    return 0;

  char *eq = strchr(p, '=');
  if (!eq)
    return 0;

  int len = eq - p;
  if (len >= KEY_LEN)
    len = KEY_LEN - 1;

  strncpy(buffer, p, len);
  buffer[len] = '\0';
  return 1;
}

void run_env_check() {
  free_list();

  FILE *f_env = fopen(".env", "r");
  if (f_env) {
    char line[MAX_LINE];
    char key[KEY_LEN];
    while (fgets(line, sizeof(line), f_env)) {
      if (parse_key(line, key))
        add_key(key);
    }
    fclose(f_env);
  }

  FILE *f_ex = fopen(".env.example", "r");
  if (!f_ex) {
    printf("Error: No existe .env.example\n");
    free_list();
    return;
  }

  int missing = 0;
  char line[MAX_LINE];
  char key[KEY_LEN];

  printf("--- Checking .env ---\n");

  while (fgets(line, sizeof(line), f_ex)) {
    if (parse_key(line, key)) {
      if (!has_key(key)) {
        printf("[MISSING] %s\n", key);
        missing++;
      }
    }
  }

  if (missing == 0)
    printf("OK: Todo sincronizado.\n");
  else
    printf("\nTotal faltantes: %d\n", missing);

  fclose(f_ex);
  free_list();
}
