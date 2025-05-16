# Nome do executável
EXEC = main.exe

# Diretórios
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include

# Arquivos fonte
SRC = $(wildcard $(SRC_DIR)/*.c)

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -g3 -I$(INCLUDE_DIR)

# Regra padrão
all: prepare $(BIN_DIR)/$(EXEC)

# Cria o diretório bin se não existir
prepare:
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Compila todos os arquivos .c e gera o executável na pasta bin
$(BIN_DIR)/$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para executar o programa
run: all
	@chcp 65001 > nul
	@$(BIN_DIR)/$(EXEC)

# Limpa os arquivos gerados
clean:
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

# Regra para exibir ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o programa"
	@echo "  make run      - Compila e executa o programa"
	@echo "  make clean    - Remove os arquivos gerados"