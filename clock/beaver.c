#define BEAVER_ALWAYS_SYNC

#include "lib/beaver.h"

#define FLAGS "-g -Wall -Werror -Og"
#define FAST_FLAGS "-Ofast -march=native"

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
        call_or_warn("./out");
    } else if (strcmp(argv[1], "recompile") == 0) {
        recompile();
    } else if (strcmp(argv[1], "clean") == 0) {
        rm("build/*");
        rm("out");
    } else if (strcmp(argv[1], "fast") == 0) {
        compile(program, FAST_FLAGS);
    } else if(strcmp(argv[1], "install") == 0) {
        call_or_panic("cp out /usr/bin/clock");
    }else {
        fprintf(stderr, "Error: unknown option: '%s'\n", argv[1]);
        exit(1);
    }
    return 0;
}
