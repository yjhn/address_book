OUTPUT_DIR:=build
BIN:=$(OUTPUT_DIR)/address_book
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:%.c=$(OUTPUT_DIR)/%.o)
CC:=gcc
# CPPFLAGS stand for 'C preprocessor flags'
CPPFLAGS:=
CFLAGS:=-std=gnu11 -Wall -Wextra -Wpedantic -Wconversion -g

.PHONY: all clean run valgrind cppcheck debug
.DELETE_ON_ERROR:

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^

# Creates a separate rule for each .o file
$(OBJS): $(OUTPUT_DIR)/%.o: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OUTPUT_DIR)

run: $(BIN)
	./$(BIN)

debug: $(BIN)
	gdb --args ./$(BIN)

valgrind: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(BIN)

format:
	clang-format -i --style=file --verbose

cppcheck:
	cppcheck --enable=all .

# compile_commands.json is needed for clangd to work
# we generate them using bear:
# https://github.com/rizsotto/Bear
clangd: Makefile
	bear -- $(MAKE) all
