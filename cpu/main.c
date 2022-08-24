#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    char cmd[]  = "iostat";
    FILE* f = popen(cmd, "r");
    uint64_t size = 0;
    char* line = NULL;

    getline(&line, &size, f);
    getline(&line, &size, f);
    getline(&line, &size, f);
    getline(&line, &size, f);

    char* s = line;

    while(isspace(*s)) {
        s++;
    }
    char* start = s;
    while(*s && !isspace(*s))
    {
        s++;
    }
    *s = 0;
    float usage = atof(start);
    printf("%.2f%%\n", usage);

    free(line);

    pclose(f);
    return 0;
}
