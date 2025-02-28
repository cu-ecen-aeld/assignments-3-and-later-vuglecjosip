CC := $(CROSS_COMPILE)gcc
CFLAGS := -Wall -Werror -g
TARGET := writer

.PHONY: all clean

# Default target
all: $(TARGET)

$(TARGET): writer.c
	$(CC) $(CFLAGS) -o $(TARGET) writer.c

# Clean target
clean:
	rm -f $(TARGET) *.o
