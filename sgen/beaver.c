#include "lib/beaver.h"

#define FLAGS "-g -Wall -Werror -Og"

module_t modules[] = {
    { .name = "main", .src = "main.c" },
    { .name = "latex_article", .src = "latex_article.c" },
};

uint32_t modules_len = sizeof(modules) / sizeof(*modules);

char* program[] = { "main", "latex_article", NULL };

int main(int argc, char** argv)
{
    auto_update(argv);

    if (argc == 1) {
        compile(program, FLAGS);
    } else if (strcmp(argv[1], "recompile") == 0) {
        recompile();
    } else if (strcmp(argv[1], "install") == 0) {
        printf("cp out /usr/bin/sgen\n");
        call_or_panic("cp out /usr/bin/sgen");
    } else if (strcmp(argv[1], "clean") == 0) {
        rm("$(find build/ -type f)");
        rm("out");
    }
    return 0;
}
