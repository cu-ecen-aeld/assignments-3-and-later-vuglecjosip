CC = gcc
CFLAGS = -Wall -Werror -g
TARGET = aesdsocket

all: $(TARGET)

$(TARGET): aesdsocket.c
	$(CC) $(CFLAGS) -o $(TARGET) aesdsocket.c

clean:
    # Ensure this line is indented with a tab
	rm -f $(TARGET)