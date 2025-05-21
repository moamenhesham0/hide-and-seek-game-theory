SOURCE_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
OUTPUT_DIR = bin
BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -Wextra -O2 \
	-I$(INCLUDE_DIR) \
	-I$(SOURCE_DIR) \
	-I$(SOURCE_DIR)/game_logic/ai_characters \
	-I$(LIB_DIR)/lp_solve_ux \
	`sdl2-config --cflags` \
	`pkg-config --cflags SDL2_image SDL2_mixer`

LDFLAGS = -L$(LIB_DIR)/lp_solve_ux -llpsolve55 \
	`sdl2-config --libs` \
	`pkg-config --libs SDL2_image SDL2_ttf SDL2_mixer` \
	-lm

SRC = $(wildcard $(SOURCE_DIR)/*.c) \
      $(wildcard $(SOURCE_DIR)/gui/*.c) \
      $(wildcard $(SOURCE_DIR)/gui/menu/*.c) \
      $(wildcard $(SOURCE_DIR)/gui/game/*.c) \
      $(wildcard $(SOURCE_DIR)/gui/game_flow/*.c) \
      $(wildcard $(SOURCE_DIR)/game_logic/*.c) \
      $(wildcard $(SOURCE_DIR)/game_logic/ai_characters/*.c)

OBJ = $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
TARGET = $(OUTPUT_DIR)/hide_seek

# Matrix-only binary
MATRIX_SRC = $(SOURCE_DIR)/solve_matrix.c
MATRIX_OBJ = $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(MATRIX_SRC))
MATRIX_TARGET = $(OUTPUT_DIR)/solve_matrix

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(MATRIX_TARGET): $(MATRIX_OBJ)
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c to .o into build/
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR)/lp_solve_ux:$$LD_LIBRARY_PATH ./$(TARGET)

run_matrix: $(MATRIX_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR)/lp_solve_ux:$$LD_LIBRARY_PATH ./$(MATRIX_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_DIR)

.PHONY: all run run_matrix clean
