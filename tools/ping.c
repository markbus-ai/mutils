#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  float min, avg, max, mdev;
  int loss;
} NetStats;

int check_dns() { return gethostbyname("google.com") != NULL; }

int get_ping_stats(const char *ip, NetStats *s) {
  char cmd[64], line[256];
  snprintf(cmd, sizeof(cmd), "ping -c 5 -q -W 2 %s", ip);

  FILE *fp = popen(cmd, "r");
  if (!fp)
    return 0;

  s->loss = 100; // Default fail
  s->avg = -1;

  while (fgets(line, sizeof(line), fp)) {
    if (strstr(line, "packet loss")) {
      // Formato linux: "5 packets transmitted, 5 received, 0% packet loss"
      char *ptr = strstr(line, "% packet loss");
      if (ptr) {
        while (*ptr != ' ' && ptr > line)
          ptr--;
        s->loss = atoi(ptr);
      }
    }
    if (strstr(line, "rtt min/avg")) {
      char *eq = strchr(line, '=');
      if (eq)
        sscanf(eq + 2, "%f/%f/%f/%f", &s->min, &s->avg, &s->max, &s->mdev);
    }
  }
  pclose(fp);
  return (s->avg != -1);
}

void run_ping_check() {
  printf("--- Network Diagnostic ---\n");

  // 1. DNS
  if (!check_dns()) {
    printf("[FAIL] DNS Resolution (Check /etc/resolv.conf)\n");
    return;
  }
  printf("[OK] DNS\n");

  // 2. Ping Stats
  NetStats s;
  printf("Testing connection to 1.1.1.1...\n");

  if (get_ping_stats("1.1.1.1", &s)) {
    printf("Loss: %d%% | Avg: %.1fms | Jitter: %.1fms\n", s.loss, s.avg,
           s.mdev);

    if (s.loss > 0)
      printf("-> PROBLEM: Packet Loss detected.\n");
    else if (s.avg > 150)
      printf("-> PROBLEM: High Latency.\n");
    else if (s.mdev > 20)
      printf("-> PROBLEM: Unstable connection (Jitter).\n");
    else
      printf("-> STATUS: Healthy.\n");
  } else {
    printf("[FAIL] No route to host.\n");
  }
}
