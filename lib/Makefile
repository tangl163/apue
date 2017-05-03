SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(obj:.o:.d)

lib.a: $(OBJ)
	$(AR) rv $@ $?

-include $(DEP)

%.d: %.c
	gcc $< -MM -MT $(@:.d=.o) >$@

clean:
	rm -f *.o *.a *.d

