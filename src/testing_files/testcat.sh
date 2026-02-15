#!/bin/bash

# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES="diff_result.txt"

# Путь к твоей программе
CAT="./cat"

# 1. Подготовка тестовых файлов
echo "Testing cat..."

# Файл с обычным текстом
echo "Simple text line 1" > test_1.txt
echo "Simple text line 2" >> test_1.txt

# Файл с пустыми строками (для -s, -n, -b)
printf "Line 1\n\n\n\nLine 5\n" > test_2.txt

# Файл с табуляциями и Windows-переносами (для -e, -E, -t, -T)
# \x0d - это \r, \x09 - это \t
printf "Tab here:\tEnd.\nWindows line\x0d\nNon-print:\x01\x02\n" > test_3.txt

# Список флагов для проверки (включая GNU длинные флаги)
flags=(
    "-b"
    "-e"
    "-n"
    "-s"
    "-t"
    "-v"
    "-E"
    "-T"
    "--number-nonblank"
    "--number"
    "--squeeze-blank"
    "-be"
    "-nst"
)

# Список файлов для проверки
files=(
    "test_1.txt"
    "test_2.txt"
    "test_3.txt"
    "test_1.txt test_2.txt test_3.txt"
)

# Функция для запуска теста
run_test() {
    param=$1
    file=$2
    (( COUNTER++ ))
    
    # Запуск твоей программы и системного cat
    $CAT $param $file > res.txt 2>/dev/null
    cat $param $file > sys_res.txt 2>/dev/null
    
    # Сравнение
    diff res.txt sys_res.txt > $DIFF_RES
    
    if [ ! -s $DIFF_RES ]; then
        (( SUCCESS++ ))
        echo -e "${GREEN}[ PASS ]${NC} Test $COUNTER: cat $param $file"
    else
        (( FAIL++ ))
        echo -e "${RED}[ FAIL ]${NC} Test $COUNTER: cat $param $file"
        echo "--- Difference ---"
        cat $DIFF_RES
        echo "------------------"
    fi
    rm -f res.txt sys_res.txt $DIFF_RES
}

# --- ОСНОВНОЙ ЦИКЛ ТЕСТОВ ---
for f in "${flags[@]}"; do
    for file in "${files[@]}"; do
        run_test "$f" "$file"
    done
done

# --- СПЕЦИАЛЬНЫЙ ТЕСТ: Ошибка (несуществующий файл) ---
(( COUNTER++ ))
$CAT non_existent.txt > /dev/null 2> s21_err.txt
EXIT_CODE=$?

if [ -s err.txt ] && [ $EXIT_CODE -ne 0 ]; then
    echo -e "${GREEN}[ PASS ]${NC} Test $COUNTER: Error handling (non-existent file)"
    (( SUCCESS++ ))
else
    echo -e "${RED}[ FAIL ]${NC} Test $COUNTER: Error handling (no stderr or exit 0)"
    (( FAIL++ ))
fi

# Итоги
echo "=============================="
echo -e "RESULT: ${GREEN}SUCCESS: $SUCCESS${NC} | ${RED}FAIL: $FAIL${NC}"
echo "=============================="

# Очистка

rm -f test_1.txt test_2.txt test_3.txt err.txt cat

if [ $FAIL -ne 0 ]; then
    exit 1
fi
