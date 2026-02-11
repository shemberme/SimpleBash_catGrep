CC = gcc
SANIT_FLAGS = -fsanitize=address -g
FLAGS =  -std=c11 -Wall -Wextra -Werror -o
PREF_SRC = ./src/
PREF_BIN = ./bin/

SRC = $(wildcard $(PREF_SRC)cat/*.c )

all: s21_cat 


s21_cat:

	$(CC) $(SRC) $(PREF_SRC)common/parser.c $(FLAGS) $(PREF_SRC)cat/s21_cat

test:
	$(CC)  $(SANIT_FLAGS) $(SRC) $(PREF_SRC)common/parser.c $(FLAGS) $(PREF_SRC)cat/s21_cat

clear:
	rm -rf $(PREF_SRC)cat/s21_cat
