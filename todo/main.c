#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/buffer.h"
#include "lib/panic.h"

typedef struct todo_t todo_t;
struct todo_t {
    char desc[128];
    char head[32];
};

#define TODO_FILE_PATH "/home/ben/.config/todo/todo"

void read_todos(buffer_t* todos)
{
    FILE* fh = fopen(TODO_FILE_PATH, "r");
    panic_if(fh == NULL, "could not open file: %s: %s", TODO_FILE_PATH,
        strerror(errno));

    todo_t* t = buffer_more(todos);
    panic_if(t == NULL, "could not allocate todo buffer: %s", strerror(errno));

    while (fread(t, 1, sizeof(*t), fh) > 0) {
        t = buffer_more(todos);
        panic_if(t == NULL, "could not allocate todo vector: %s",
            strerror(errno));
    }

    // remove last added thing
    {
        uint64_t pos = ((uint8_t*)buffer_end(todos) - (uint8_t*)buffer_begin(todos)) - 1;
        buffer_remove(todos, pos);
    }

    fclose(fh);
}

void write_todos(buffer_t* todos)
{
    FILE* fh = fopen(TODO_FILE_PATH, "w");
    panic_if(fh == NULL, "could not open file: %s: %s", TODO_FILE_PATH,
        strerror(errno));

    todo_t* iter = (todo_t*)buffer_begin(todos);
    todo_t* end = (todo_t*)buffer_end(todos);
    for (; iter != end; iter++) {
        fwrite(iter, 1, sizeof(*iter), fh);
    }

    fclose(fh);
}

bool is_uint(char* s)
{
    for (; *s; s++) {
        if (!isdigit(*s)) {
            return 0;
        }
    }
    return 1;
}

void remove_todo(buffer_t* todos, char* arg)
{
    uint64_t pos;
    if (is_uint(arg)) {
        pos = atol(arg);
        if (pos >= buffer_size(todos)) {
            panic("unknown todo id: %lu", pos);
        }

    } else {
        todo_t* iter = (todo_t*)buffer_begin(todos);
        todo_t* end = (todo_t*)buffer_end(todos);
        for (; iter != end; iter++) {
            if (strcmp(iter->head, arg) == 0) {
                pos = iter - (todo_t*)buffer_begin(todos);
                break;
            }
        }
        panic_if(iter == end, "unknown todo: %s", arg);
    }
    buffer_remove(todos, pos);
}

void add_todo(buffer_t* todos, char** argv)
{
    panic_if(*argv == 0, "you need at least a header for your todo");

    todo_t* t = buffer_more(todos);
    panic_if(t == NULL, "could not allocate todo buffer: %s", strerror(errno));
    memset(t, 0, sizeof(*t));

    // header
    {
        uint64_t len = strlen(*argv);
        panic_if(len >= sizeof(t->head), "head: '%s' is too long", *argv);
        memcpy(t->head, *argv, len);
        argv++;
    }

    // desc
    {
        if (*argv == NULL) {
            return;
        }
        uint64_t len = strlen(*argv);
        panic_if(len >= sizeof(t->desc), "description: '%s' is too long", *argv);
        memcpy(t->desc, *argv, len);
        argv++;
    }
}

void print_todos(buffer_t* todos)
{

    // TODO: nice colors
    printf("Your todos:\n");

    todo_t* iter = buffer_begin(todos);
    todo_t* end = buffer_end(todos);
    uint64_t pos = 0;
    for (pos = 0; iter != end; iter++, pos++) {
        printf("\t[%lu] %s: %s\n", pos, iter->head, iter->desc);
    }

    if (pos == 0) {
        printf("\t---none---\n");
    }
}

void parse_argv(buffer_t* todos, char** argv)
{
    if (*argv == NULL) {
        print_todos(todos);
        return;
    } else if (strcmp(*argv, "add") == 0) {
        add_todo(todos, argv + 1);
    } else if (strcmp(*argv, "rm") == 0) {
        remove_todo(todos, argv[1]);
    } else {
        panic("unknown option: %s", argv[0]);
    }
}

int main(int argc, char** argv)
{
    buffer_t* todos = buffer_create(8, sizeof(todo_t));
    read_todos(todos);
    parse_argv(todos, &argv[1]);
    write_todos(todos);
    buffer_free(todos);
    return 0;
}
