SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# ifndef CFLAGS
CFLAGS = -c
# endif

lib.a: $(OBJ)
	$(AR) rv $@ $?

%.o: %.c
	gcc -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f *.o *.a

