CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
TARGET = cat
CPPCHECK = cppcheck --enable=all --suppress=missingIncludeSystem --std=c11
SRC = $(wildcard *.c )
OBJS = $(SRC: .c = .o )
TEST = ../testing_files/testcat.sh

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: 
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)
	chmod +x $(TEST)
	$(TEST)

cppcheck:
	$(CPPCHECK) $(SRC)

leak: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) -benst $(TARGET).c
style: 
	clang-format -n *.c *.h
style-fix:
	clang-format -i *.c *.h


clean:
	rm -rf $(TARGET)

rebuild: clean all style

.PHONY: all test cppcheck style clean rebuild leak style-fix
