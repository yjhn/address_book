LIB_DIR:=lib
BIN_DIR:=src
LIB_NAME:=libaddressbook.so
BIN_NAME:=address_book
LIB:=$(LIB_DIR)/$(LIB_NAME)
BIN:=$(BIN_DIR)/$(BIN_NAME)
EXEC_ENV:=LD_LIBRARY_PATH=$(LIB_DIR)
EXEC_BIN:=$(EXEC_ENV) ./$(BIN)
LIB_INSTALL_DIR:=/usr/local/lib/x86_64-linux-gnu/
BIN_INSTALL_DIR:=/usr/local/bin/

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

install: $(BIN)
	sudo mkdir -p $(LIB_INSTALL_DIR) $(BIN_INSTALL_DIR)
	sudo cp $(LIB) $(LIB_INSTALL_DIR)
	sudo cp $(BIN) $(BIN_INSTALL_DIR)
	sudo ldconfig

uninstall:
	sudo rm -f $(LIB_INSTALL_DIR)$(LIB_NAME) $(BIN_INSTALL_DIR)$(BIN_NAME)
	sudo ldconfig

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
