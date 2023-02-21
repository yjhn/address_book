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
	cd $(BIN_DIR) && $(MAKE)

$(LIB):
	cd $(LIB_DIR) && $(MAKE)

clean:
	cd $(LIB_DIR) && $(MAKE) clean
	cd $(BIN_DIR) && $(MAKE) clean
	git clean -dfX

run: $(BIN)
	$(EXEC_BIN)

debug: $(BIN)
	$(EXEC_ENV) gdb --args ./$(BIN)

valgrind: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes -s env $(EXEC_BIN)

format:
	cd $(LIB_DIR) && $(MAKE) format
	cd $(BIN_DIR) && $(MAKE) format

cppcheck:
	cppcheck --enable=all .

check:
	cd $(LIB_DIR) && $(MAKE) check
	cd $(BIN_DIR) && $(MAKE) check

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
