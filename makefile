# =============================
# Compiler Selection
# =============================

CC ?= gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -O2 -Iincludes
# Linker flags
LDFLAGS =

# Output executable
TARGET = build/MorseCodeTranslator

# Source and object files
SRC_DIR = src
OBJ_DIR = build/obj
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# =============================
# Build Rules
# =============================

.PHONY: all clean gcc clang debug dirs

all: dirs $(TARGET)

# Ensure output directories exist
dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p build

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile each .c into build/obj/*.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# =============================
# Convenience Targets
# =============================

# Force GCC
gcc:
	$(MAKE) CC=gcc clean all

# Force Clang
clang:
	$(MAKE) CC=clang clean all

# Debug build (no optimisation, debug symbols)
debug:
	$(MAKE) CFLAGS="-Wall -Wextra -Wpedantic -std=c17 -g -Iincludes" clean all

# =============================
# Cleanup
# =============================

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
