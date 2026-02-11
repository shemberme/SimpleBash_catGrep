#ifndef COMMON_PARSER_H
#define COMMON_PARSER_H

typedef struct opts {
  int b, e, n, s, t, v;
  /* data */
} Flags;

void flags_on(char* argv, Flags* opts);
void openfile_with_flag(char* filename, Flags* opts);

#endif