#define CBUILD
#include "lib/cbuild.h"

#define FLAGS "-g -Wall -pedantic"
#define FAST_FLAGS " -Ofast -march=native"
#define PROFILE_FLAGS " -pg -Wall -pedantic"

void compile_all(char* flags)
{
    compile_object("build/main.o", flags, "main.c");
    compile_object_directory("out", flags, "", "build/");
}

void clean(void)
{
    rm("build/*");
    rm("out");
}

int main(int argc, char** argv)
{
    auto_update();
    if(argc == 1) {
        compile_all(FLAGS);
    } else if(strcmp(argv[1], "profile") == 0) {
         compile_all(PROFILE_FLAGS);
    } else if(strcmp(argv[1], "fast") == 0) {
        clean();
        compile_all(FAST_FLAGS);
    } else if(strcmp(argv[1], "clean") == 0) {
        clean();
    } else if(strcmp(argv[1], "install") == 0) {
        printf("cp out /usr/bin/clock\n");
        system("cp out /usr/bin/clock");
    }else {
        fprintf(stderr, "\033[31mError: \033[39m unknown option: %s\n", argv[1]);
    }
    return 0;
}
