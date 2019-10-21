
OFLAGS=-O2

.PHONY: clean all


all: main inline separatefile

main: mainfile.c
	gcc $(OFLAGS) -o main mainfile.c
inline: inline.c
	gcc $(OFLAGS) -o inline inline.c

separatefile: separatefile.o cpx.o
	gcc -o separatefile separatefile.o cpx.o

separatefile.o: separatefile.c cpx.h
	gcc $(OFLAGS) -c separatefile.c
cpx.o: cpx.c cpx.h
	gcc $(OFLAGS) -c cpx.c

clean:
	rm -f *.o







