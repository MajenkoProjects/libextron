CFLAGS=-I.

all: libextron.a test

libextron.a: extron.o
	ar rcs libextron.a extron.o

extron.o: extron.c

test: test.o libextron.a
	cc -o test test.o -L. -lextron

clean:
	rm -f test test.o lbextron.a extron.o
