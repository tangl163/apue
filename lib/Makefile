SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

lib.a: $(OBJ)
	$(AR) rv $@ $?

-include $(DEP)

%.d: %.c
	@gcc $< -MM -MT $(@:.d=.o) > $@

.PHONY: clean
clean:
	rm -f *.o *.a *.d

