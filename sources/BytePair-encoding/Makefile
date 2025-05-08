.PHONY: all clean format debug release

# default build type is release
BUILD ?= release

CC = gcc
CFLAGS_COMMON = -Wall -g -Wextra -std=c2x 
LDFLAGS = 

# Conditional flags based on build type
ifeq ($(BUILD),debug)
	CFLAGS = $(CFLAGS_COMMON) -g -DDEBUG
	TARGET = bpe_debug
else ifeq ($(BUILD),release)
	CFLAGS = $(CFLAGS_COMMON) -O2
	TARGET = bpe
else
	$(error Unknown build type: $(BUILD))
endif

SRC_DIR = src

SRC = $(wildcard $(SRC_DIR)/*.c)

all: $(TARGET)

$(TARGET): $(SRC)
	@$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)


debug:
	$(MAKE) BUILD=debug

release:
	$(MAKE) BUILD=release

clean:
	@rm -f bpe bpe_debug

format:
	@echo "Execute clang-format on src/*.{c,h}"
	@find . -type f -path "./src/*.c" -o -path "./src/*.h" | xargs clang-format -i
