#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pars.h"

int main(int argc, char* argv[]) {
  // инициализация структуры с флагами равными нулю. Тоесть выключены.
  Flags opts = {0, 0, 0, 0, 0, 0};
  int line_num = 1;
  int res = 0;

  if (flags_on(argc, argv, &opts) != 0) {
    return 1;
  }
  if (optind == argc) {
    open_file(stdin, &opts, &line_num);
  } else {
    for (int i = optind; i < argc; i++) {
      res = openfile_with_flag(argv[i], &opts, &line_num);
    }
  }
  return res;
}
