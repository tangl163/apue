ROOT = ..
include $(ROOT)/Make.defines

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

lib.a: $(OBJ)
	$(AR) rv $@ $?

%.o: %.c
	$(CC) $(DEBUG) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o *.a

