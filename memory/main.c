#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char cmd[] = "free --mebi";
    FILE* meminfo = popen(cmd, "r");

    uint64_t size = 0;
    char* line = NULL;

    getline(&line, &size, meminfo);
    getline(&line, &size, meminfo);

    uint64_t total = 0;
    uint64_t used = 0;
    // parsing
    {
        char* s = line;     
        char* start;
        while(isspace(*s)) {
            s++;
        }
        while(*s && !isspace(*s)) {
            s++;
        }
        while(isspace(*s)) {
            s++;
        }
        // s is on total
        start = s;
        while(*s && !isspace(*s)) {
            s++;
        }
        *s = 0;
        total = atol(start);
        s++;

        // s is on used
        while(isspace(*s)) {
            s++;
        }
        start = s;
        while(*s && !isspace(*s)) {
            s++;
        }
        *s = 0;
        used = atol(start);
        s++;
    }

    printf("%luM / %luM\n", used, total);

    // here could be swap space usage be implemented
    free(line);
    pclose(meminfo);
    return 0;
}
