ROOT = ..
include $(ROOT)/Make.defines

SRC = $(wildcard *.c)
TARGET = $(SRC:.c=)


.PHONY: all
all:
	for i in $(TARGET); do \
		($(CC) $(DEBUG) $(CFLAGS) -o $$i $$i.c $(LIBARC)) || exit 1; \
	done

