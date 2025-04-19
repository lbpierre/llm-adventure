CC = gcc
CFLAGS = -Wall -g -Wextra -std=c2x
LDFLAGS = -lpcre2-8

# Define the target binary name
TARGET = reader
SRC_DIR = src

SRC = $(wildcard $(SRC_DIR)/*.c)

all: $(TARGET)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm $(TARGET)
