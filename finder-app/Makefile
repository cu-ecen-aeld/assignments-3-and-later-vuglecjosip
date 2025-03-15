CC := $(CC)
CPPFLAGS := $(CPPFLAGS)
CFLAGS := $(CLAGS)
TARGET := writer

.PHONY: all clean

# Default target
all: $(TARGET)

$(TARGET): writer.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(TARGET) writer.c

# Clean target
clean:
	rm -f $(TARGET) *.o
