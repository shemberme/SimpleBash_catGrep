#ifndef COMMON_PARS_H
#define COMMON_PARS_H

#include <stdio.h>

typedef struct opts {
  int b, e, n, s, t, v;
  /* data */
} Flags;

void open_file(FILE* file, const Flags* opts, int* line_num);
int flags_on(const int argc, char* argv[], Flags* opts);
int openfile_with_flag(const char* filename, const Flags* opts,
                       int* count_line);

#endif
