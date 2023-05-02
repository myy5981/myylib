CC = gcc -m64
CLEVEL = -O2
CFLAGS = -Wall -Werror -I./include $(CLEVEL) -std=c99 -c

OBJ = obj/endian.o\
		obj/hex.o\
		obj/sm3_hmac.o\
		obj/sm3_kdf.o\
		obj/sm3.o\
		obj/sm4.o\
		obj/url_base64.o\
		obj/algorithm.o\
		obj/bignum.o

default: mkdir build

debug: CLEVEL = -g -fdiagnostics-color=always
debug: default

build: lib/libmyy.a lib/libmyy.so

lib/libmyy.a: $(OBJ)
	ar rcs lib/libmyy.a $?

lib/libmyy.so: $(OBJ)
	gcc -fPIC -shared $^ -o lib/libmyy.so

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean: 
	rm -rf bin/** obj/** lib/**

.PHONY: mkdir
mkdir: 
	@mkdir -p obj bin lib
