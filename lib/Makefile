LIB:=libaddressbook.so
SRCS:=$(wildcard *.c)
HEADERS:=$(wildcard *.h)
OBJS:=$(SRCS:.c=.o)
ASSEMBLY_FILES:=$(wildcard *.s)
CFLAGS:=-std=gnu11 -Wall -Wextra -Wpedantic -Wconversion -g -fPIC

.PHONY: all clean format cppcheck
.DELETE_ON_ERROR:

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^
	
# Creates a separate rule for each .o file
# CPPFLAGS stand for 'C preprocessor flags'
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(LIB) $(ASSEMBLY_FILES)

# Produces .s files.
check:
	gcc $(CPPFLAGS) $(CFLAGS) -fanalyzer -S $(SRCS)

format:
	clang-format -i --style=file --verbose $(SRCS) $(HEADERS)

cppcheck:
	cppcheck --enable=all .
