#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/parser.h"

int main(int argc, char* argv[]) {

  // инициализация структуры с флагами равными нулю. Тоесть выключены.
  Flags opts = {0, 0, 0, 0, 0, 0};
  flags_on(argc,argv,&opts);
  
  for (int i = optind; i < argc; i++) {
    openfile_with_flag(argv[i], &opts);
  }
}
