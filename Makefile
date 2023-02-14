.PHONY: run build valgrind format

run: build
	./build/addr

build:
	mkdir -p build
	gcc -o build/addr main.c -std=gnu11 -Wall -Wextra -Wpedantic -Wconversion

valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all -s ./build/addr

format:
	clang-format -i --style=file *.c *.h --verbose
