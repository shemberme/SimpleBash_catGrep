#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void flags_on(char *argv,Flags *opts){

    for(int j = 1; argv[j] != '\0';j++){
        if (!strcmp(argv,"-n")) opts->n = 1;
        else if(!strcmp(argv,"--number")){
            //printf("Флаг : %s Активирован.", argv[i]);
            opts->n = 1;
            continue;
           }
        else if (!strcmp(argv,"-b") || !strcmp(argv, "--number-nonblank")) opts->b = 1;
        else if (!strcmp(argv,"-e") || !strcmp(argv,"-E")) opts->e = 1;
        else if (!strcmp(argv,"-s") || !strcmp(argv, "--squeeze-blank")) opts->s = 1;
        else if (!strcmp(argv,"-t") || !strcmp(argv,"-T")) opts->t = 1;
        else {
            fprintf(stderr, "Неизвестный флаг: %s\n", argv);
            break;
        }
        // if(*flag_count < MAX_FLAGS){
        // // Здесь обновляем количество флагов
        // (*flag_count)++; 
        // }
        
    }
    if(opts->b) opts->n = 0; // Если есть флаг -b , то флаг -n выключаем.
}

void openfile_with_flag(char *filename,Flags *opts){
    //char buffer[1024];
    int ch = 0;
    int line_num = 1;
    int is_start_of_line = 1;
    int last_ch = '\n';       // Храним предыдущий символ (по умолчанию начало файла как новая строка)
    int empty_lines = 0;
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "cat: %s: No such file or directory\n", filename);
        return;
    }
    while((ch = fgetc(f)) != EOF) {
        
        if(ch == '\n' && last_ch == '\n'){
            if(is_start_of_line) 
                empty_lines++;
        }else empty_lines = 0;

        if(opts->s && empty_lines > 1){
            continue;
        }

        if(is_start_of_line){
            if(opts->n || (opts->b && ch != '\n')){
                printf("%6d\t",line_num++);
            }
        }

        if(opts->e && ch == '\n'){
            if(is_start_of_line && (opts->b)) printf("\t$");
            else {printf("$");} 

        }
        if(opts->t && ch == '\t'){
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