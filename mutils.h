// mutils.h
#ifndef MUTILS_H
#define MUTILS_H
// Prototipos de las herramientas
void run_port_slayer(char *port);
void run_bat_threshold(int *max);
void run_bat_health();
void run_nuke_dev(char *path);
void run_env_check();
void run_ping_check();
void run_myip();
void run_extract(const char *filename);
void run_compress(const char *filename, const char *output);
void run_ocr();
int run_cmd(const char *file, char *const argv[]);
int run_cmd_redirect(const char *file, char *const argv[], const char *out_path);
int mutils_strcasecmp(const char *s1, const char *s2); // implementacion de strcmp que ignora mayúsculas y minúsculas
#endif
