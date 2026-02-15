#include "pars.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int flags_on(const int argc, char* argv[], Flags* opts) {
  int opt;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--number-nonblank") == 0)
      argv[i] = "-b";
    else if (strcmp(argv[i], "--number") == 0)
      argv[i] = "-n";
    else if (strcmp(argv[i], "--squeeze-blank") == 0)
      argv[i] = "-s";
  }

  while ((opt = getopt(argc, argv, "beEvnstT")) != -1) {
    switch (opt) {
      case 'b':
        opts->b = 1;
        opts->n = 0;
        break;
      case 'e':
        opts->e = 1;
        opts->v = 1;
        break;
      case 'E':
        opts->e = 1;
        break;
      case 'n':
        if (!opts->b) opts->n = 1;
        break;
      case 's':
        opts->s = 1;
        break;
      case 't':
        opts->t = 1;
        opts->v = 1;
        break;
      case 'T':
        opts->t = 1;
        break;
      case 'v':
        opts->v = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [-beEvnstT] [file...]\n", argv[0]);
        return 1;
    }
  }
  return 0;
}

void open_file(FILE* file, const Flags* opts, int* line_num) {
  int last_ch = '\n';
  int empty_lines = 0;
  int ch = 0;
  // Читаем файл
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n' && last_ch == '\n') {
      empty_lines++;
    } else {
      empty_lines = 0;
    }
    // Пропуск пустых строк
    if (opts->s && empty_lines > 1) {
      continue;
    }

    // Печать номера строки
    if (last_ch == '\n') {
      if (opts->b) {
        if (ch != '\n') printf("%6d\t", (*line_num)++);
      } else if (opts->n) {
        printf("%6d\t", (*line_num)++);
      }
    }

    if (ch == '\n') {
      if (opts->e) printf("$");  // Печатаем $ ПЕРЕД переходом на новую строку
      putchar(ch);
    } else if (ch == '\t') {
      if (opts->t)
        printf("^I");
      else
        putchar(ch);
    } else if (ch == '\r' && opts->e && !opts->v) {
      printf("^M");
    } else if (opts->v) {
      // Стандартная логика -v (печать спецсимволов)
      if (ch < 32)
        printf("^%c", ch + 64);
      else if (ch == 127)
        printf("^?");
      else if (ch > 127) {
        printf("M-");
        unsigned char tmp = (unsigned char)ch & 0x7F;
        if (tmp < 32)
          printf("^%c", tmp + 64);
        else if (tmp == 127)
          printf("^?");
        else
          putchar(tmp);
      } else
        putchar(ch);
    } else {
      // Обычный символ
      putchar(ch);
    }
    last_ch = ch;
  }
}
int openfile_with_flag(const char* filename, const Flags* opts, int* line_num) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", filename);
    return 1;
  }
  open_file(file, opts, line_num);
  fclose(file);
  return 0;
}
