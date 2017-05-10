# Local makefile.

LIBDIR = ./lib
LIBARC = $(LIBDIR)/lib.a
TARGET = a.out

$(TARGET): object.o lib
	gcc -o $@ $< $(LIBARC)

object.o: object.c common.h
	gcc -c object.c

lib:
	$(MAKE) -C $(LIBDIR)

clean:
	rm -f *.out *.c *.o

