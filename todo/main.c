#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lib/buffer.h"
#include "lib/panic.h"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define MAGENTA "\033[95m"
#define BLUE "\033[96m"
#define YELLOW "\033[93m"
#define RESET "\033[0m"

typedef struct todo_t todo_t;
struct todo_t {
  char body[64];
  bool has_date;
  struct tm date;
};

#define TODO_FILE_PATH "/home/ben/.config/todo/todo"

void read_todos(buffer_t *todos) {
  FILE *fh = fopen(TODO_FILE_PATH, "r");
  panic_if(fh == NULL, "could not open file: %s: %s", TODO_FILE_PATH,
           strerror(errno));

  todo_t *t = buffer_more(todos);
  panic_if(t == NULL, "could not allocate todo buffer: %s", strerror(errno));

  while (fread(t, 1, sizeof(*t), fh) > 0) {
    t = buffer_more(todos);
    panic_if(t == NULL, "could not allocate todo vector: %s", strerror(errno));
  }

  // remove last added thing
  {
    uint64_t pos =
        ((todo_t *)buffer_end(todos) - (todo_t *)buffer_begin(todos)) - 1;
    buffer_remove(todos, pos);
  }

  fclose(fh);
}

void write_todos(buffer_t *todos) {
  FILE *fh = fopen(TODO_FILE_PATH, "w");
  panic_if(fh == NULL, "could not open file: %s: %s", TODO_FILE_PATH,
           strerror(errno));

  todo_t *iter = (todo_t *)buffer_begin(todos);
  todo_t *end = (todo_t *)buffer_end(todos);
  for (; iter != end; iter++) {
    fwrite(iter, 1, sizeof(*iter), fh);
  }

  fclose(fh);
}

bool is_uint(char *s) {
  for (; *s; s++) {
    if (!isdigit(*s)) {
      return 0;
    }
  }
  return 1;
}

void remove_todo(buffer_t *todos, char *arg) {
  panic_if(!is_uint(arg), "argument of remove must be a number");
  uint64_t pos = atol(arg);
  if (pos >= buffer_size(todos)) {
    panic("unknown todo id: %lu", pos);
  }
  buffer_remove(todos, pos);
}

uint8_t day_name(int8_t d, int8_t m, int16_t y) {
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

void stotm(struct tm *t, char *s) {
  if (!isdigit(s[0]) || !isdigit(s[1]) || s[5] != '.' || !isdigit(s[3]) ||
      !isdigit(s[4]) || s[5] != '.' || !isdigit(s[6]) || !isdigit(s[7]) ||
      !isdigit(s[8]) || !isdigit(s[9]) || s[10] != '\0') {
    panic("'%s' is not a date: expected format: dd.mm.yyyy", s);
  }

  s[2] = 0;
  s[5] = 0;
  t->tm_mday = atoi(&s[0]);
  t->tm_mon = atoi(&s[3]) - 1;
  t->tm_year = atoi(&s[6]) - 1900;
}

void add_todo(buffer_t *todos, char **argv) {
  panic_if(*argv == 0, "you need at least a header for your todo");

  todo_t *t = buffer_more(todos);
  panic_if(t == NULL, "could not allocate todo buffer: %s", strerror(errno));
  memset(t, 0, sizeof(*t));

  // header
  {
    uint64_t len = strlen(*argv);
    panic_if(len >= sizeof(t->body), "head: '%s' is too long", *argv);
    memcpy(t->body, *argv, len);
    argv++;
  }
  if (*argv == NULL) {
    return;
  }
  // date
  {
    t->has_date = 1;
    stotm(&t->date, *argv);
  }
}

int date_diff(const struct tm *a, const struct tm *b) {
  int year_diff = a->tm_year - b->tm_year;
  int mon_diff = a->tm_mon - b->tm_mon;
  int mday_diff = a->tm_mday - b->tm_mday;

  if (year_diff != 0) {
    return year_diff;
  }
  if (mon_diff != 0) {
    return mon_diff;
  }
  if (mday_diff != 0) {
    return mday_diff;
  }
  return 0;
}

void format_date(struct tm *now, struct tm *date) {
  int d = date_diff(now, date);
  if (d == 0) {
    printf(YELLOW);
  } else if (d > 0) {
    printf(RED);
  } else {
    printf(GREEN);
  }

  fprintf(stdout, "%02d.%02d.%04d" RESET, date->tm_mday, date->tm_mon + 1,
          date->tm_year + 1900);
}

void print_todos(buffer_t *todos) {

  todo_t *iter = buffer_begin(todos);
  todo_t *end = buffer_end(todos);

  time_t now = time(NULL);
  struct tm now_format = {0};
  localtime_r(&now, &now_format);

  uint64_t pos = 0;
  for (pos = 0; iter != end; iter++, pos++) {
    printf(" [" BLUE "%02lu" RESET "] ", pos);
    if (iter->has_date) {
      format_date(&now_format, &iter->date);
    } else {
      printf("          ");
    }
    printf(" %s\n", iter->body);
  }

  if (pos == 0) {
    printf("\t---none---\n");
  }
}

void find_todos(buffer_t *todos, char **argv) {
  // clang-format off
    struct tm desired = { 0 };
    struct tm now     = { 0 };
  // clang-format on

  time_t now_ = time(NULL);
  localtime_r(&now_, &now);

  stotm(&desired, *argv);
  todo_t *iter = buffer_begin(todos);
  todo_t *end = buffer_end(todos);
  int d = 0;
  uint64_t pos = 0;
  for (; iter != end; iter++) {
    if (!iter->has_date) {
      continue;
    }

    d = date_diff(&desired, &iter->date);
    if (d == 0) {
      printf(" [" BLUE "%02lu" RESET "] ", pos);
      format_date(&now, &iter->date);
      printf(" %s\n", iter->body);
      pos++;
    }
  }
}

void parse_argv(buffer_t *todos, char **argv) {
  if (*argv == NULL) {
    print_todos(todos);
    return;
  } else if (strcmp(*argv, "add") == 0) {
    add_todo(todos, argv + 1);
  } else if (strcmp(*argv, "rm") == 0) {
    remove_todo(todos, argv[1]);
  } else if (strcmp(*argv, "r") == 0) {
    remove_todo(todos, argv[1]);
  } else if (strcmp(*argv, "a") == 0) {
    add_todo(todos, argv + 1);
  } else if (strcmp(*argv, "find") == 0) {
    find_todos(todos, argv + 1);
  } else if (strcmp(*argv, "f") == 0) {
    find_todos(todos, argv + 1);
  } else {
    panic("unknown option: %s", argv[0]);
  }
}

int todo_cmp(const void *x, const void *y) {
  const todo_t *a = x;
  const todo_t *b = y;
  if (!a->has_date && !b->has_date) {
    return strcmp(a->body, b->body);
  }
  if (!a->has_date) {
    return -1;
  }

  if (!b->has_date) {
    return 1;
  }
  int d = date_diff(&a->date, &b->date);
  if (d == 0) {
    return strcmp(a->body, b->body);
  }
  return d;
}

int main(int argc, char **argv) {
  buffer_t *todos = buffer_create(8, sizeof(todo_t));
  read_todos(todos);
  parse_argv(todos, &argv[1]);

  qsort(buffer_begin(todos), buffer_size(todos), sizeof(todo_t), todo_cmp);

  write_todos(todos);
  buffer_free(todos);
  return 0;
}
