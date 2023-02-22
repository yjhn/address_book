BIN:=address_book
SRCS:=$(wildcard *.c)
HEADERS:=$(wildcard *.h)
OBJS:=$(SRCS:.c=.o)
ASSEMBLY_FILES:=$(wildcard *.s)
LIB_PATH:=../lib/
# CPPFLAGS stand for 'C preprocessor flags'
CPPFLAGS:=-I$(LIB_PATH)
CFLAGS:=-std=gnu11 -Wall -Wextra -Wpedantic -Wconversion -g
LDLIBS:=-laddressbook
LDFLAGS:=-L$(LIB_PATH)

.PHONY: all clean format cppcheck debug
.DELETE_ON_ERROR:

all: $(BIN)

# -l option must be passed after all the other files
$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Creates a separate rule for each .o file
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(BIN) $(ASSEMBLY_FILES)

# Produces .s files.
check:
	gcc $(CPPFLAGS) $(CFLAGS) -fanalyzer -S $(SRCS)

format:
	clang-format -i --style=file --verbose $(SRCS) $(HEADERS)

cppcheck:
	cppcheck --enable=all .
