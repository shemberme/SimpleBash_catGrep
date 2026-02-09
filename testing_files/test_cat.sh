#!/bin/bash

# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Путь к твоей программе
MY_CAT=".././bin/main"

# Создаем тестовые файлы, если их нет
echo -e "line 1\n\n\nline 2\n\nline 3" > test_s.txt
echo -e "text\twith\ttabs" > test_t.txt
echo -e "    \n  space line\n" > test_b.txt

# Список флагов для тестов
flags=("-n" "-b" "-s" "-e" "-t" "-ne" "-st" "-nb" "-ns")

echo "--- STARTING CAT TESTS ---"

for flag in "${flags[@]}"
do
    echo -n "Testing flag $flag: "
    
    # Запускаем твой cat и оригинальный cat
    $MY_CAT $flag test_s.txt test_t.txt test_b.txt > my_res.txt
    cat $flag test_s.txt test_t.txt test_b.txt > orig_res.txt
    
    # Сравниваем результаты
    DIFF_RES="$(diff my_res.txt orig_res.txt)"
    
    if [ -z "$DIFF_RES" ]; then
        echo -e "${GREEN}SUCCESS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "$DIFF_RES" # Показать разницу, если тест провален
    fi
done

# Очистка временных файлов
rm my_res.txt orig_res.txt
