LIB_DIR:=lib
BIN_DIR:=src
LIB:=$(LIB_DIR)/libaddressbook.so
BIN:=$(BIN_DIR)/address_book
EXEC_ENV:=LD_LIBRARY_PATH=$(LIB_DIR)
EXEC_BIN:=$(EXEC_ENV) ./$(BIN)

.PHONY: all clean $(LIB) format clangd infer run valgrind cppcheck debug
.DELETE_ON_ERROR:

all: $(BIN)

$(BIN): $(LIB)
	$(MAKE) -C $(BIN_DIR)

$(LIB):
	$(MAKE) -C $(LIB_DIR)

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(BIN_DIR) clean


run: $(BIN)
	$(EXEC_BIN)

debug: $(BIN)
	$(EXEC_ENV) gdb --args ./$(BIN)

valgrind: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes -s env $(EXEC_BIN)

format:
	$(MAKE) -C $(LIB_DIR) format
	$(MAKE) -C $(BIN_DIR) format

cppcheck:
	cppcheck --enable=all .

check:
	$(MAKE) -C $(LIB_DIR) check
	$(MAKE) -C $(BIN_DIR) check

# compile_commands.json is needed for clangd to work
# we generate them using bear:
# https://github.com/rizsotto/Bear
# Clean build artifacts first to force make to run the build commands.
clangd: clean
	bear -- $(MAKE)

# Infer static analyzer: https://fbinfer.com
# REMOVE -fanalyzer COMPILER FLAG BEFORE RUNNING, clang DOES NOT
# HAVE IT
infer: clean
	infer run -- $(MAKE)
