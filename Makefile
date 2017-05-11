# Local makefile.

LIBDIR = ./lib
LIBARC = $(LIBDIR)/lib.a
TARGET = a.out

$(TARGET): object.o lib
	gcc -o $@ $< $(LIBARC)

object.o: object.c common.h
	gcc -c object.c

.PHONY: lib
lib:
	$(MAKE) -C $(LIBDIR)

.PHONY: clean
clean:
	rm -f *.out *.c *.o

