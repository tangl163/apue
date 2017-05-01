# Local makefile

a.out: common.o  current.o
	gcc common.o current.o

common.o: common.c common.h
	gcc -c common.c

current.o: current.c common.h
	gcc -c current.c

clean:
	rm a.out current.o

