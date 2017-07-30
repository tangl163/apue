# Local makefile.

LIBDIR = ./lib
LIBARC = $(LIBDIR)/lib.a
TARGET = a.out
export CFLAGS = -c -Wall -Wextra -Werror -Wmissing-prototypes -Wstrict-prototypes

ifdef DEBUG
CFLAGS += -g
endif

$(TARGET): object.o lib
	gcc -o $@ $< $(LIBARC)

object.o: object.c common.h
	gcc $(CFLAGS) object.c

.PHONY: lib
lib:
	$(MAKE) -C $(LIBDIR)

.PHONY: clean
clean:
	rm -f *.out *.o
	$(MAKE) -C $(LIBDIR) clean

