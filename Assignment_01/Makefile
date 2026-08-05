CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -Iinclude -O2
SRC_DIR  := src
OUT_DIR  := output
TEST_DIR := tests

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(SRCS))
TARGET := $(OUT_DIR)/matrix_project

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

run: all
	./$(TARGET)

test: all
	@bash run_tests.sh

clean:
	rm -f $(OUT_DIR)/*.o $(TARGET)
	rm -f $(OUT_DIR)/*.out

.DELETE_ON_ERROR: