#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FLAGS 20
#define MAX_FILES 20
/*
Принцип работы утилиты cat:
1. Конструкция команды cat [OPTION][-belnstuv]... [FILE]...
2. используемые флаги 
    Опции cat
    №	Опции	Описание
    1	-b (GNU: --number-nonblank)	нумерует только непустые строки
    2	-e (предполагает -v), -E (GNU only: без применения -v)	отображает символы конца строки как $; для \r\n отображает ^M$
    3	-n (GNU: --number)	нумерует все выходные строки
    4	-s (GNU: --squeeze-blank)	сжимает несколько смежных пустых строк
    5	-t предполагает и -v (GNU: -T то же самое, но без применения -v)	также отображает табы как ^I

*/

typedef struct opts{
    int b,e,n,s,t;
    /* data */
} Flags;

void openfile(char *filename);
void flags_on(char *argv,int *flag_count,Flags *opts);
void openfile_with_flag(char *filename,Flags *opts);

int main(int argc, char *argv[]){
    //char *flags[MAX_FLAGS] = {NULL};   // Массив для флагов
    char *files[MAX_FILES] = {NULL};   // Массив для файлов
    Flags opts = {0,0,0,0,0}; // инициализация структуры с необходимыми данными
    int flag_count = 0;
    int file_count = 0;
    

    for(int i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            /*
            тут происходит вызов функции flags_on в которой происходит обновление данных в структуре с флагами.
            */
            flags_on(argv[i],&flag_count,&opts);
            

        }else if(file_count < MAX_FILES){
            files[file_count] = argv[i];
            file_count++;
        }

    }
    
    //int files_count = (sizeof(files)/sizeof(files[0]));
    //printf("Размер массива с файлами --> %d\n", file_count);
    //-----------------------ОТЛАДКА--------------------
    // printf("Найденные флаги (%d): \n", flag_count);
    // for(int i = 0;i < flag_count;i++){
    //     printf(" %s\n",flags[i]);
    // }
    // printf("Найденные файлы (%d): \n",file_count);
    // for(int i = 0;i < file_count;i++){
    //     printf(" %s\n",files[i]);
    // }

    //-----------------------ОТЛАДКА--------------------
    for(int i = 0; i < file_count;i++){
        openfile_with_flag(files[i],&opts);
    }

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
            printf("$");

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

void flags_on(char *argv,int *flag_count,Flags *opts){

    for(int j = 1; argv[j] != '\0';j++){
        if (argv[j] == 'n') opts->n = 1;
        else if (argv[j] == 'b') opts->b = 1;
        else if (argv[j] == 'e') opts->e = 1;
        else if (argv[j] == 's') opts->s = 1;
        else if (argv[j] == 't') opts->t = 1;
        else {
            fprintf(stderr, "Неизвестный флаг: %c\n", argv[j]);
        }
        if(*flag_count < MAX_FLAGS){
        // Здесь обновляем количество флагов
        (*flag_count)++; 
        }
        
    }
    if(opts->b) opts->n = 0; // Если есть флаг -b , то флаг -n выключаем.
}
/*
при переборе аргументов, нужно сначала найти файл для чтения , потом уже обрабатывать флаги
например:
            ./s21_cat -n file1.txt -v file2.txt
сначала нужно открыть file1.txt и file2.txt, а потом уже применять флаги.

так же утилита должна обрабатывать сразу все флаги не противоречащие друг другу.
и так же, думаю любое количество файлов выводить, объединять, и редактировать. в зависимости от выбранного флага
. Мы должны работать с массивоми данных, Тоесть.
./s21_cat -n -b -v test.txt test2.txt test3.txt > result.txt
Что должно произойти. 
в программе, читаем все аргументы. 
Все аргументы с - в начале, заносим в отдельный масив данных. flags[?] = {"-n","-b","-v"};
Все файлы заносим в другой масив files[?] = {"test.txt","test2.txt","test3.txt"};
Указатель > Который означает перенаправленые вывода, используем отдельно. и вместе с ним 
так же используем то куда мы отправляем данные, тоесть открываем файл result.txt на запись.
И с учётом флагов записываем все данные по очерёдно проходя по массиву, files открываем, читаем , записываем 
всё содержимое в result.txt. Закрываем test.txt. и так далее, пока в files не останется данных. 
так же необходимо применить к каждому файлу который мы используем, флаги из масива flags. 

*/