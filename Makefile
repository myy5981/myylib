.DEFAULT_GOAL := dynamic

CC = gcc -m64
LINK = gcc -m64
AR = ar
CLEVEL = -O2
CFLAGS = -c -Wall -Werror -I./include $(CLEVEL)

OBJ = obj/endian.o\
		obj/hex.o\
		obj/sm3_hmac.o\
		obj/sm3_kdf.o\
		obj/sm3.o\
		obj/sm4.o\
		obj/url_base64.o\
		obj/algorithm.o\
		obj/bignum.o\
		obj/bignum_512.o

.PHONY: debug
debug: CLEVEL = -g -fdiagnostics-color=always
debug: $(.DEFAULT_GOAL)

.PHONY: release
release: $(.DEFAULT_GOAL)

.PHONY: static
static: mkdir lib/libmyy.a

.PHONY: dynamic
dynamic: mkdir lib/libmyy.so

lib/libmyy.a: $(OBJ)
	$(AR) rcs $@ $?

lib/libmyy.so: $(OBJ:.o=.pic.o)
	$(LINK) -shared $^ -o $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $^ -o $@

obj/%.pic.o: src/%.c
	$(CC) $(CFLAGS) -fPIC $^ -o $@

.PHONY: clean
clean: 
	rm -rf bin obj lib

.PHONY: mkdir
mkdir: 
	@mkdir -p obj bin lib
