.PHONY: run build valgrind format

run: build
	./build/addr

build:
	mkdir -p build
	gcc -o build/addr main.c -std=gnu11 -Wall -Wextra -Wpedantic -Wconversion

valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all -s ./build/addr

format:
	clang-format -i --style=file main.c address_book.c --verbose

# compile_commands.json is needed for clangd to work
# we generate them using bear:
# https://github.com/rizsotto/Bear
clangd:
	bear -- make build