CFLAGS=-I.

DESTDIR?=/usr

all: libextron.a test

libextron.a: extron.o
	ar rcs libextron.a extron.o

extron.o: extron.c

test: test.o libextron.a
	cc -o test test.o -L. -lextron

clean:
	rm -f test test.o lbextron.a extron.o

install: libextron.a
	install -m 644 libextron.a ${DESTDIR}/lib
	install -m 644 extron.h ${DESTDIR}/include
