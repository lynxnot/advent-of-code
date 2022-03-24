CC          := cc
CFLAGS      := -Wall -Werror -pedantic -std=c11
DBGFLAGS    := -g
CCOBJFLAGS  := $(CFLAGS) -c
BIN_PATH    := bin
TARGET_NAME := aoc2021-d02
TARGET      := $(BIN_PATH)/$(TARGET_NAME)

default: makedir $(TARGET) run

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH)

.PHONY: run
run:
	@$(TARGET)

.PHONY: clean
clean:
	@rm -f $(TARGET)
	@rmdir $(BIN_PATH)