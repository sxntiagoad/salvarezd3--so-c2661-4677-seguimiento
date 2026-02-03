CC=gcc
CFLAGS=-Wall

all: ambiente variable

ambiente: ambiente.c
	$(CC) $(CFLAGS) ambiente.c -o ambiente.exe

variable: variable.c
	$(CC) $(CFLAGS) variable.c -o variable.exe

clean:
	del ambiente.exe variable.exe
