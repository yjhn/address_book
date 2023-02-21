BIN:=address_book
SRCS:=$(wildcard *.c)
HEADERS:=$(wildcard *.h)
OBJS:=$(SRCS:.c=.o)
# CPPFLAGS stand for 'C preprocessor flags'
CPPFLAGS:=
CFLAGS:=-std=gnu11 -Wall -Wextra -Wpedantic -Wconversion -g -fanalyzer

.PHONY: all clean run valgrind cppcheck debug
.DELETE_ON_ERROR:

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^
	
# Creates a separate rule for each .o file
$(OBJS): %.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(BIN)
	git clean -dfX

run: $(BIN)
	./$(BIN)

debug: $(BIN)
	gdb --args ./$(BIN)

valgrind: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(BIN)

format:
	clang-format -i --style=file --verbose $(SRCS) $(HEADERS)

cppcheck:
	cppcheck --enable=all .

# compile_commands.json is needed for clangd to work
# we generate them using bear:
# https://github.com/rizsotto/Bear
# Clean build artifacts first to force make to run the build commands.
clangd: clean
	bear -- $(MAKE) all

# Infer static analyzer: https://fbinfer.com
# REMOVE -fanalyzer COMPILER FLAG BEFORE RUNNING, clang DOES NOT
# HAVE IT
infer: clean
	infer run -- $(MAKE)
