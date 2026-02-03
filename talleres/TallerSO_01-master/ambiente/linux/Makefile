CC=gcc
CFLAGS=-Wall -std=c11

all: ambiente variable

ambiente: ambiente.c
	$(CC) $(CFLAGS) ambiente.c -o ambiente

variable: variable.c
	$(CC) $(CFLAGS) variable.c -o variable

clean:
	rm -f ambiente variable
