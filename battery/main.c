#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    UNKNOWN,
    CHARGING,
    FULL,
    DISCHARGING,
} status_t;

status_t get_status()
{
    FILE* fh = fopen("/sys/class/power_supply/BAT1/status", "r");
    char* line = NULL;
    uint64_t len = 0;
    getline(&line, &len, fh);
    fclose(fh);

    status_t r = UNKNOWN;
    if (strncmp("Full", line, 4) == 0) {
        r = FULL;
    } else if (strncmp("Charging", line, 8) == 0) {
        r = CHARGING;
    } else if (strncmp("Discharging", line, 11) == 0) {
        r = DISCHARGING;
    }

    free(line);
    return r;
}

int main(int argc, char** argv)
{
    FILE* fh = fopen("/sys/class/power_supply/BAT1/capacity", "r");

    char* line = NULL;
    uint64_t line_size = 0;
    uint64_t line_len = getline(&line, &line_size, fh);
    line[line_len - 1] = '\0';

    {
        status_t status = get_status();
        switch (status) {
        case UNKNOWN:
            printf("~");
            break;
        case CHARGING:
            printf("+");
            break;
        case FULL:
            break;
        case DISCHARGING:
            printf("-");
            break;
        }
    }

    printf("%s%%\n", line);
    free(line);
    fclose(fh);
    return 0;
}
