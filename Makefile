SOURCE_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
OUTPUT_DIR = bin

CC = gcc
CFLAGS = -Wall -Wextra -O2 -I$(INCLUDE_DIR) -I$(SOURCE_DIR) -I$(LIB_DIR)/lp_solve_ux `sdl2-config --cflags` `pkg-config --cflags SDL2_image`
LDFLAGS = -L$(LIB_DIR)/lp_solve_ux -llpsolve55 `sdl2-config --libs` `pkg-config --libs SDL2_image`
SRC = $(wildcard $(SOURCE_DIR)/*.c $(SOURCE_DIR)/gui/*.c)
OBJ = $(SRC:.c=.o)
TARGET = $(OUTPUT_DIR)/hide_seek

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean
