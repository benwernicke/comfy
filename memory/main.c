#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH "/proc/meminfo"

#define ERROR_IF(b, ...)                                                       \
    {                                                                          \
        if (b) {                                                               \
            fprintf(stderr, "error: ");                                        \
            fprintf(stderr, __VA_ARGS__);                                      \
            fprintf(stderr, "\n");                                             \
            exit(1);                                                           \
        }                                                                      \
    }

int main(void)
{
    uint64_t total = 0;
    uint64_t avail = 0;
    uint64_t freem = 0;

    FILE* fh = fopen(PATH, "r");
    ERROR_IF(fh == NULL, "could not open file: %s: %s", PATH, strerror(errno));
    char* line = NULL;
    uint64_t size = 0;
    int64_t len = 0;

    // total
    {
        len = getline(&line, &size, fh);
        ERROR_IF(0 >= len, "could not read line: %s", strerror(errno));
        char* s = strchr(line, ':');
        s++;
        while (isspace(*s)) {
            s++;
        }
        char* e = s;
        while (*e && !isspace(*e)) {
            e++;
        }
        *e = 0;
        total = atol(s);
    }
    // free skipping
    {
        len = getline(&line, &size, fh);
        ERROR_IF(0 >= len, "could not read line: %s", strerror(errno));
        char* s = strchr(line, ':');
        s++;
        while (isspace(*s)) {
            s++;
        }
        char* e = s;
        while (*e && !isspace(*e)) {
            e++;
        }
        *e = 0;
        freem = atol(s);
    }
    // available
    {
        len = getline(&line, &size, fh);
        ERROR_IF(0 >= len, "could not read line: %s", strerror(errno));
        char* s = strchr(line, ':');
        s++;
        while (isspace(*s)) {
            s++;
        }
        char* e = s;
        while (*e && !isspace(*e)) {
            e++;
        }
        *e = 0;
        avail = atol(s);
    }

    free(line);
    fclose(fh);

    avail /= 1024;
    total /= 1024;
    freem /= 1024;

    printf("%lu, %lu / %lu\n", total - freem, total - avail, total);

    return 0;
}
