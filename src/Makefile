BIN:=address_book
SRCS:=$(wildcard *.c)
HEADERS:=$(wildcard *.h)
OBJS:=$(SRCS:.c=.o)
ASSEMBLY_FILES:=$(wildcard *.s)
# CPPFLAGS stand for 'C preprocessor flags'
CPPFLAGS:=
CFLAGS:=-std=gnu11 -Wall -Wextra -Wpedantic -Wconversion -g
LIB_PATH:=../lib/
LDLIBS:=-laddressbook
LDFLAGS:=-L$(LIB_PATH)

.PHONY: all clean format cppcheck debug
.DELETE_ON_ERROR:

all: $(BIN)

# -l option must be passed after all the other files
$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Creates a separate rule for each .o file
$(OBJS): %.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(LIB_PATH) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(BIN) $(ASSEMBLY_FILES)

# Produces .s files.
check:
	gcc $(CPPFLAGS) $(CFLAGS) -I$(LIB_PATH) -fanalyzer -S $(SRCS)

format:
	clang-format -i --style=file --verbose $(SRCS) $(HEADERS)

cppcheck:
	cppcheck --enable=all .
