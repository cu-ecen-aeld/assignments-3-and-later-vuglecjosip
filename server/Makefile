CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -g -Wall -Werror
TARGET ?= aesdsocket

all: aesdsocket

${TARGET}: ${TARGET}.c
	$(CC) $(CFLAGS) -o ${TARGET} ${TARGET}.c 


clean:
	rm -f *.o ${TARGET}
