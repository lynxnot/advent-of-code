CC           := gcc
CFLAGS       := -Wall -Werror -pedantic -std=c11 
DBGFLAGS     := -g
CCOBJFLAGS   := $(CFLAGS) -c
BIN_PATH     := bin
TARGET_NAME  := aoc2021-d04
TARGET       := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(TARGET).dbg

default: makedir $(TARGET) run

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $<


$(TARGET_DEBUG): main.c
	$(CC) $(CFLAGS) $(DBGFLAGS) -o $@ $<

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH)

.PHONY: debug
debug: makedir $(TARGET_DEBUG)

.PHONY: run
run:
	@$(TARGET)

.PHONY: clean
clean:
	@rm -f $(TARGET)
	@rmdir $(BIN_PATH)