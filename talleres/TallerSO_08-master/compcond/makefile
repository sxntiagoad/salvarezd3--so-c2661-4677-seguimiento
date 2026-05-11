CC      = gcc
CFLAGS  = -Wall

.PHONY: all clean release debug

all: compcond

compcond: compcond.c
	$(CC) $(CFLAGS) -DVERSION=\"1.0\" -o $@ $<

debug: compcond.c
	$(CC) $(CFLAGS) -DDEBUG -DVERSION=\"1.0-debug\" -o compcond $<

release: compcond.c
	$(CC) $(CFLAGS) -O2 -DVERSION=\"1.0-release\" -o compcond $<

clean:
	rm -f compcond
