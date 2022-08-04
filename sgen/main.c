#include "lib/panic.h"
#include "latex_article.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Function Definitions
// ----------------------------------------------------------------------------

void clang_format_onoff(char** argv);
void help(char** argv);

// Type Definitions
// ----------------------------------------------------------------------------
typedef void (*option_cbs_t)(char** argv);

typedef struct option_t option_t;
struct option_t {
    char name[32];
    option_cbs_t cbs;
};

option_t options[] = {
    // clang-format off
    { .name = "clang_format",   .cbs = clang_format_onoff },
    { .name = "latex_article",  .cbs = latex_article },

    { .name = "help",           .cbs = help },
    { .name = "--help",         .cbs = help },
    { .name = "-h",             .cbs = help },
    // clang-format on
};

uint64_t options_len = sizeof(options) / sizeof(option_t);

void parse_argv(char** argv)
{
    option_t* iter = options;
    for (; iter != options + options_len; iter++) {
        if (strcmp(*argv, iter->name) == 0) {
            iter->cbs(argv + 2);
        }
    }
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        help(NULL);
    } else {
        parse_argv(argv + 1);
    }
    return 0;
}

// Function Implementation
// ----------------------------------------------------------------------------

void clang_format_onoff(char** argv)
{
    printf("// clang-format off\n");
    printf("// clang-format on\n");
}

void help(char** argv)
{
    printf("Available: \n");
    option_t* iter = options;
    for (; iter != options + options_len; iter++) {
        printf("\t%s\n", iter->name);
    }
}
