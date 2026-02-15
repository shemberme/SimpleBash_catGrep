#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e, i, v, c, l, n, h, s, f, o;
  char pattern[8192];
  int pattern_len;
} Flags;

void parse_flags(int argc, char** argv, Flags* flags);
void add_pattern(Flags* flags, const char* pattern);
void add_pattern_from_file(Flags* flags, const char* filepath);
void process_file(Flags flags, const char* filename, int file_count,
                  regex_t* re);
void output_line(char* line, int n);

#endif
