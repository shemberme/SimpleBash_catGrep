#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""

# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

declare -a tests=(
    "VAR main grep.c"
    "VAR -e int grep.c"
    "VAR -e int -e void grep.c"
    "VAR -e int -e void grep.c grep.h"
    "VAR -i main grep.c"
    "VAR -v main grep.c"
    "VAR -c main grep.c"
    "VAR -l main grep.c grep.h"
    "VAR -n main grep.c"
    "VAR -h main _grep.c grep.h"
    "VAR -s nonexistent_file grep.c"
    "VAR -o main grep.c"
    "VAR -iv main grep.c"
    "VAR -in main grep.c"
    "VAR -cl main grep.c grep.h"
    "VAR -no main grep.c"
    "VAR -nh main grep.c grep.h"
    "VAR -e \"^int\" grep.c"
    "VAR -e \"}$\" grep.c"
)

# Создаем файл для теста -f
echo "main" > pattern_file.txt
echo "void" >> pattern_file.txt
tests+=( "VAR -f pattern_file.txt grep.c" )
tests+=( "VAR -if pattern_file.txt grep.c" )

echo "--- STARTING GREP TESTS ---"

run_test() {
    # Заменяем VAR на комбинацию флагов
    param=$(echo "$1" | sed "s/VAR/$2/")
    
    # Запуск оригинального grep и нашего
    grep $param > grep_origin.log
    ./grep $param > grep.log
    
    DIFF_RES="$(diff -q grep_origin.log grep.log)"
    
    if [ -z "$DIFF_RES" ]; then
        (( SUCCESS++ ))
        echo -e "[${GREEN}SUCCESS${NC}] $param"
    else
        (( FAIL++ ))
        echo -e "[${RED}FAILED${NC}] $param"
        echo "Diff detected:"
        diff grep_origin.log grep.log | head -n 5
    fi
    rm grep_origin.log grep.log
}

# 1. Тесты одиночных флагов и простых комбинаций
for test_case in "${tests[@]}"; do
    run_test "$test_case" ""
done

# 2. Дополнительные комбинации (динамически)
echo "--- TESTING COMBINATIONS ---"
run_test "VAR -inc main grep.c" ""
run_test "VAR -inv main grep.c" ""
run_test "VAR -l -e void grep.c grep.h" ""
run_test "VAR -c -h main grep.c grep.h" ""
run_test "VAR -on main grep.c" ""

# Очистка
rm pattern_file.txt grep_origin.log grep.log grep


echo "--- TESTS FINISHED ---"
echo -e "Total: $((SUCCESS + FAIL))"
echo -e "Success: ${GREEN}$SUCCESS${NC}"
echo -e "Failed: ${RED}$FAIL${NC}"

if [ $FAIL -ne 0 ]; then
    exit 1
fi
