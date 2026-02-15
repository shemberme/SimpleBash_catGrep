#define _GNU_SOURCE
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

void add_pattern(Flags* flags, const char* pattern) {
  if (flags->pattern_len > 0) {
    strcat(flags->pattern + flags->pattern_len, "|");
    flags->pattern_len++;
  }
  if (strlen(pattern) == 0) {
    strcat(flags->pattern + flags->pattern_len, ".");
  } else {
    strcat(flags->pattern + flags->pattern_len, "(");
    strcat(flags->pattern + flags->pattern_len, pattern);
    strcat(flags->pattern + flags->pattern_len, ")");
  }
  flags->pattern_len = strlen(flags->pattern);
}

void add_pattern_from_file(Flags* flags, const char* filepath) {
  FILE* fp = fopen(filepath, "r");
  if (!fp) {
    if (!flags->s) fprintf(stderr, "grep: %s: No such file\n", filepath);
    return;
  }
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, fp)) != -1) {
    if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';
    add_pattern(flags, line);
  }
  free(line);
  fclose(fp);
}

void process_file(Flags flags, const char* filename, int file_count,
                  regex_t* re) {
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    if (!flags.s) fprintf(stderr, "grep: %s: No such file\n", filename);
    return;
  }

  char* line = NULL;
  size_t len = 0;
  int line_num = 0, matches = 0;

  while (getline(&line, &len, fp) != -1) {
    line_num++;

    size_t newline_pos = strcspn(line, "\n");
    char saved_char = line[newline_pos];
    line[newline_pos] = '\0';

    regmatch_t pmatch;
    int result = regexec(re, line, 1, &pmatch, 0);
    int is_match = (result == 0 && !flags.v) || (result != 0 && flags.v);

    if (is_match) {
      matches++;
      if (!flags.c && !flags.l) {
        if (flags.o && !flags.v) {
          char* ptr = line;
          while (regexec(re, ptr, 1, &pmatch, 0) == 0) {
            if (file_count > 1 && !flags.h) printf("%s:", filename);
            if (flags.n) printf("%d:", line_num);
            printf("%.*s\n", (int)(pmatch.rm_eo - pmatch.rm_so),
                   ptr + pmatch.rm_so);
            ptr += pmatch.rm_eo;
            if (pmatch.rm_so == pmatch.rm_eo) ptr++;
          }
        } else {
          if (file_count > 1 && !flags.h) printf("%s:", filename);
          if (flags.n) printf("%d:", line_num);
          printf("%s\n", line);
        }
      }
    }
    line[newline_pos] = saved_char;
  }

  if (flags.c && !flags.l) {
    if (file_count > 1 && !flags.h) printf("%s:", filename);
    printf("%d\n", matches);
  }

  if (flags.l && matches > 0) {
    printf("%s\n", filename);
  }

  free(line);
  fclose(fp);
}

int main(int argc, char** argv) {
  Flags flags = {0};  // Инициализация всех полей нулями
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        flags.e = 1;
        add_pattern(&flags, optarg);
        break;
      case 'i':
        flags.i = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'c':
        flags.c = 1;
        break;
      case 'l':
        flags.l = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 'h':
        flags.h = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 'f':
        flags.f = 1;
        add_pattern_from_file(&flags, optarg);
        break;
      case 'o':
        flags.o = 1;
        break;
    }
  }

  if (!flags.e && !flags.f && optind < argc) {
    add_pattern(&flags, argv[optind++]);
  }

  if (optind >= argc) return 0;

  regex_t re;
  int cflags = REG_EXTENDED;
  if (flags.i) cflags |= REG_ICASE;

  if (regcomp(&re, flags.pattern, cflags) != 0) {
    if (!flags.s) fprintf(stderr, "grep: error\n");
    return 1;
  }

  int file_count = argc - optind;
  for (int i = optind; i < argc; i++) {
    process_file(flags, argv[i], file_count, &re);
  }

  regfree(&re);
  return 0;
}
