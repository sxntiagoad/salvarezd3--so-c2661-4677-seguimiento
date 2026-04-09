.phony: clean

SRCS=proceso-demonio.c
OBJS=$(SRCS:.c=.o)
EXECS=proceso-demonio

all: ${EXECS}

proceso-demonio: proceso-demonio.o
	${CC} -o $@ $<

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
servidor3: servidor3.c
	cc -o servidor3 servidor3.c
