#include "lib/beaver.h"

#define FLAGS "-g -Wall -Werror -Og"

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
    }
    return 0;
}
