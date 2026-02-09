CC = gcc
FLAGS = -std=c11 -Wall -Wextra -Werror -o
PREF_SRC = ./src/
PREF_BIN = ./bin/

SRC = $(wildcard $(PREF_SRC)*.c)

all:
	$(CC) $(SRC) $(FLAGS) $(PREF_BIN)main


clear:
	rm -rf $(PREF_BIN)main
