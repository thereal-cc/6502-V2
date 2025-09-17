# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Isrc -g

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Create bin directory
$(shell mkdir -p $(BIN_DIR))

# Automatically find all .c files in src/ and subdirectories
SRC = $(shell find $(SRC_DIR) -type f -name '*.c')

# Generate object file list in obj directory, mirroring src structure
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/6502

.PHONY: all clean

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# Compile .c files to .o files in the obj directory, ensuring obj subdirectories exist
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)