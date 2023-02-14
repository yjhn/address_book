C_FILES = main.c address_book.c

.PHONY: run valgrind format

run: build
	./build/addr

build: $(C_FILES)
	mkdir -p build
	gcc -o build/addr main.c -std=gnu11 -Wall -Wextra -Wpedantic -Wconversion

valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all -s ./build/addr

format: 
	clang-format -i --style=file $(C_FILES) --verbose

cppcheck: $(C_FILES)
	cppcheck --enable=all $(C_FILES)

# compile_commands.json is needed for clangd to work
# we generate them using bear:
# https://github.com/rizsotto/Bear
clangd: Makefile
	bear -- make build