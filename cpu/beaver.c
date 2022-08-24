#include "lib/beaver.h"

#define FLAGS "-pipe -g -Wall -Werror -Og"
#define FAST_FLAGS "-pipe -O2 -march=native"


module_t modules[] = {
    { .name = "main", .src = "main.c" },
};
uint32_t modules_len = sizeof(modules) / sizeof(*modules);

char* program[] = { "main", NULL };

int main(int argc, char** argv)
{
    auto_update(argv);
    if (argc == 1) {
        compile(program, FLAGS);
    } else if (strcmp(argv[1], "clean") == 0) {
        rm("build/*");
        rm("out");
    } else if(strcmp(argv[1], "install") == 0) {
        call_or_panic("cp out /usr/bin/cpu");
    } else if(strcmp(argv[1], "fast") == 0) {
        compile(program, FAST_FLAGS);
    }
    return 0;
}
