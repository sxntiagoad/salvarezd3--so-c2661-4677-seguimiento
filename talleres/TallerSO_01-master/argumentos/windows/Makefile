CC=gcc
CFLAGS=-Wall

all: argumentos manejo_argumentos

argumentos: argumentos.c
	$(CC) $(CFLAGS) argumentos.c -o argumentos.exe

manejo_argumentos: manejo_argumentos.c
	$(CC) $(CFLAGS) manejo_argumentos.c -o manejo_argumentos.exe

clean:
	del argumentos.exe manejo_argumentos.exe
