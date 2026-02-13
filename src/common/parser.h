#ifndef COMMON_PARSER_H
#define COMMON_PARSER_H

typedef struct opts {
  int b, e, n, s, t, v;
  /* data */
} Flags;

int flags_on (const int argc, char *argv[] ,Flags *opts);
void openfile_with_flag(const char *filename,const Flags *opts);

#endif