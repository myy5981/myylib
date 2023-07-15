CC = gcc -m64
CFLAGS = -Wall -Werror -I../include -I../../GmSSL/include -g -std=c99 -c

default: build debug

debug: debug.o
	gcc -L../lib $^ -o $@ -lmyy -lcrypto -lgmssl

debug.o: debug.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf debug debug.o
	make clean -C ../

.PHONY: build
build: 
	make debug -C ../