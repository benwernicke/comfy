#include <errno.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/panic.h"

// TODO: fixme

char* ltrim(char* s)
{
    while (*s && isspace(*s)) {
        s++;
    }
    return s;
}

int main(int argc, char** argv)
{
    panic_if(argv[1] == 0, "need a file argument");
    FILE* fh = fopen(argv[1], "r");
    panic_if(fh == NULL, "could not open file: %s: %s", argv[1],
        strerror(errno));

    char* line = NULL;
    uint64_t line_len = 0;

    char* todo = NULL;
    uint64_t line_num = 0;

    while (getline(&line, &line_len, fh) >= 0) {
        todo = strstr(line, "TODO");
        if (todo != NULL) {
            printf("%s:%lu:%lu:\t%s", argv[1], line_num, todo - line,
                ltrim(line));
        }
        line_num++;
    }
    putc('\n', stdout);

    free(line);

    fclose(fh);
    return 0;
}
