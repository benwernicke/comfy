#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
  time_t now_ = time(NULL);
  struct tm now = {0};
    localtime_r(&now_, &now);
    char buf[32] = {0};
    strftime(buf, sizeof(buf), "%a %d.%m %H:%M:%S", &now);
    puts(buf);
  return 0;
}
