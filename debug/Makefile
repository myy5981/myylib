default: build libtask.so

libtask.so: task3.c
	gcc -m64 -Wall -Werror -I../include -L../lib -g --shared -fPIC $^ -o $@ -lmyy

.PHONY: clean
clean:
	rm -rf libtask.so
	make clean -C ../

.PHONY: build
build: 
	make debug -C ../

debug: debug.c libtask.so
	gcc -g -L./ $^ -o $@ -ltask
