#define BEAVER_ALWAYS_SYNC

#include "lib/beaver.h"

#define FLAGS "-g -Wall -Werror -Og"

module_t modules[] = {
    { .name = "main", .src = "main.c" },
    { .name = "main", .src = "lib/buffer.c" },
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
    } else if (strcmp(argv[1], "install") == 0) {
        printf("cp out /usr/bin/todo\n");
        system("cp out /usr/bin/todo");
    } else {
        fprintf(stderr, "Error: unknown option '%s'\n", argv[1]);
        exit(1);
    }
    return 0;
}
