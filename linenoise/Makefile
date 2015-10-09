include ../../Makefile.comm

INC += -I./

OBJ = linenoise.o

liblinenoise.a : $(OBJ)
	ar -rus $@ $^

clean:
	rm -rf *.o *.a

