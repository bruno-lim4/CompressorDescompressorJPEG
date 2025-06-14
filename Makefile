# Diretórios
SRC_DIR := src
BIN_DIR := bin
OUT_DIR := output
COMMON_DIR := $(SRC_DIR)/common
COMPRESSOR_DIR := $(SRC_DIR)/compressor
DECOMPRESSOR_DIR := $(SRC_DIR)/descompressor

# Compilador e flags
CC := gcc
CFLAGS := -Wall -Wextra -I$(COMMON_DIR) -I$(COMPRESSOR_DIR) -I$(DECOMPRESSOR_DIR)

# Arquivos-fonte comuns
COMMON_SRC_COMPRESS := \
    $(COMMON_DIR)/bloco.c \
    $(COMMON_DIR)/consts.c \
    $(COMMON_DIR)/header.c \
    $(COMMON_DIR)/util.c \
    $(COMMON_DIR)/imagem.c \
    $(COMMON_DIR)/imagem_comprimir.c

COMMON_SRC_DECOMPRESS := \
    $(COMMON_DIR)/bloco.c \
    $(COMMON_DIR)/consts.c \
    $(COMMON_DIR)/header.c \
    $(COMMON_DIR)/util.c \
    $(COMMON_DIR)/imagem.c \
    $(COMMON_DIR)/imagem_descomprimir.c

# Arquivos do compressor
COMPRESSOR_SRC := $(SRC_DIR)/compressor.c \
                  $(wildcard $(COMPRESSOR_DIR)/*.c) \
                  $(COMMON_SRC_COMPRESS)

# Arquivos do descompressor
DECOMPRESSOR_SRC := $(SRC_DIR)/descompressor.c \
                    $(wildcard $(DECOMPRESSOR_DIR)/*.c) \
                    $(COMMON_SRC_DECOMPRESS)

# Alvos
all: $(BIN_DIR)/compressor $(BIN_DIR)/descompressor

$(BIN_DIR)/compressor: $(COMPRESSOR_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/descompressor: $(DECOMPRESSOR_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Limpeza agressiva
clean:
	rm -rf $(BIN_DIR)

# Ajuda
help:
	@echo "Alvos disponíveis:"
	@echo "  make            - Compila compressor e descompressor"
	@echo "  make clean      - Remove binários, arquivos de saída e diretórios"
	@echo "  make help       - Exibe esta ajuda"

.PHONY: all clean help
