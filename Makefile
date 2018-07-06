# Local makefile.

ROOT = .
include $(ROOT)/Make.defines
TARGET = a.out

ifdef THREAD
PTHREAD = -pthread
endif

ifdef DEBUG
export DEBUG = -g
endif

$(TARGET): object.o lib
	$(CC) -o $@ $< $(LIBARC) $(PTHREAD) $(LDFLAGS)

object.o: object.c
	$(CC) $(DEBUG) -c $(CFLAGS) object.c

.PHONY: lib
lib:
	$(MAKE) -C $(LIBDIR)

.PHONY: bin
bin:
	$(MAKE) -C $(BINDIR)

.PHONY: clean
clean:
	rm -f *.out *.o
	$(MAKE) -C $(LIBDIR) clean
	$(MAKE) -C $(BINDIR) clean

