#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int flags_on (const int argc, char *argv[] ,Flags *opts){
  int opt;

  for (int i = 1; i < argc; i++) {

    if (strcmp(argv[i], "--number-nonblank") == 0)
      argv[i] = "-b";
    else if (strcmp(argv[i], "--number") == 0)
      argv[i] = "-n";
    else if (strcmp(argv[i], "--squeeze-blank") == 0)
      argv[i] = "-s";
  }
  /*
    Начало работы с флагами с помощью функции getopt()/

  */
  // for (int i = 0; i < argc; i++) printf("argv[%d] = %s\n", i, argv[i]); 
  while ((opt = getopt(argc, argv, "beEvnstT")) != -1) {
    switch (opt) {
      case 'b':
        opts->b = 1;
        if (opts->n) opts->n = 0;
        break;
      case 'e':
        opts->e = 1;
        opts->v = 1;
        break;
      case 'E':
        opts->e = 1;
        break;
      case 'n':
        if (opts->b != 1) {
          opts->n = 1;
        }
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
      default:
        fprintf(stderr, "Usage: %s [-beEvnstT] [file...]\n", argv[0]);
        return 1;
    }
  }
  return 0;
}
void openfile_with_flag(const char *filename,const Flags *opts) {
  // char buffer[1024];
  int ch = 0;
  int line_num = 1;
  int is_start_of_line = 1;
  int last_ch = '\n'; // Храним предыдущий символ (по умолчанию начало файла
                      // как новая строка)
  int empty_lines = 0;
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    perror(filename); 
    return; 
}
  while ((ch = fgetc(f)) != EOF) {
    if (ch == '\n' && last_ch == '\n') {
      if (is_start_of_line)
        empty_lines++;
    } else
      empty_lines = 0;

    if (opts->s && empty_lines > 1) {
      continue;
    }

    if (is_start_of_line) {
      if (opts->n || (opts->b && ch != '\n')) {
        printf("%6d\t", line_num++);
      }
    }
    if (opts->v && ch == '\n' && last_ch == '\r') {
      printf("^M");
    }
    if (opts->e && ch == '\n') {
      if (is_start_of_line && (opts->b))
        printf("\t$");
      else {
        printf("$");
      }
    }
    if (opts->t && ch == '\t') {
      printf("^I");
      is_start_of_line = 0;
      last_ch = ch;
      continue;
    }

    putchar(ch);

    last_ch = ch;
    is_start_of_line = (ch == '\n');
  }
  fclose(f);
}