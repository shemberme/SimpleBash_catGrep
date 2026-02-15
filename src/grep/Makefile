CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
TARGET = grep
CPPCHECK = cppcheck --enable=all --suppress=missingIncludeSystem --std=c11
SRC = $(wildcard *.c )
OBJS = $(SRC: .c = .o )
TEST = ../testing_files/testgrep.sh

all: $(TARGET) 

$(TARGET): $(OBJS) 
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: 
	rm -rf $(TARGET)
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)
	chmod +x $(TEST)
	$(TEST)
	rm -rf $(TARGET)
cppcheck:
	$(CPPCHECK) $(SRC)

leak: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./grep -iv int grep.c
	rm -rf $(TARGET)
style: 
	clang-format -n *.c *.h

style-fix:
	clang-format -i *.c *.h

clean:
	rm -rf $(TARGET)

rebuild: clean all style-fix

.PHONY: all test cppcheck style style-fix clean rebuild leak
