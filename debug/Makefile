CC = gcc -m64
CFLAGS = -Wall -Werror -I../include -g -std=c99 -c

default: build debug

debug: debug.o
	gcc -L../lib $^ -o $@ -Wl,-Bstatic -lmyy -Wl,-Bdynamic

debug.o: debug.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf debug debug.o

.PHONY: build
build: 
	make debug -C ../