# Local makefile.

LIBDIR = ./lib
LIBARC = $(LIBDIR)/lib.a
TARGET = a.out
export CFLAGS = -c -Wall -Wextra -Werror -Wmissing-prototypes -Wstrict-prototypes

ifdef THREAD
PTHREAD = -pthread
endif

ifdef DEBUG
CFLAGS += -g
endif

ifdef LDFLAGS
LDFLAGS += -lrt
endif

$(TARGET): object.o lib
	gcc -o $@ $(PTHREAD) $(LDFLAGS) $< $(LIBARC)

object.o: object.c common.h
	gcc $(CFLAGS) object.c

.PHONY: lib
lib:
	$(MAKE) -C $(LIBDIR)

.PHONY: clean
clean:
	rm -f *.out *.o
	$(MAKE) -C $(LIBDIR) clean

