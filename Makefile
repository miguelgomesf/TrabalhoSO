CC = gcc
CFLAGS = -Wall -pthread
INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
MAT_DIR = ./arquivosmatrizes

MAIN_EXEC = $(BIN_DIR)/main_exec

# Lista de arquivos-fonte
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/semthread.c $(SRC_DIR)/comthread.c $(SRC_DIR)/execsemthread.c $(SRC_DIR)/execcomthread.c

# Lista de arquivos-objeto
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compilação dos arquivos-objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | create_dirs
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

all: create_dirs $(MAIN_EXEC) programa

# Compilação do executável principal
$(MAIN_EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

create_dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/main_exec programa

# Script wrapper para adicionar os caminhos corretos aos arquivos
programa: 
	echo '#!/bin/bash' > programa
	echo 'if [ $$2 -eq 100 ]; then' >> programa
	echo '  DIR=$(MAT_DIR)/arquivos100x100' >> programa
	echo 'elif [ $$2 -eq 1000 ]; then' >> programa
	echo '  DIR=$(MAT_DIR)/arquivos1000x1000' >> programa
	echo 'else' >> programa
	echo '  echo "Argumento inválido: $$2. Use 100 ou 1000." >&2' >> programa
	echo '  exit 1' >> programa
	echo 'fi' >> programa
	echo '$(MAIN_EXEC) $$1 $$2 $$DIR/$$3 $$DIR/$$4 $$DIR/$$5 $$DIR/$$6 $$DIR/$$7' >> programa
	chmod +x programa

.PHONY: run
run: programa
	@./programa $(ARGS)
