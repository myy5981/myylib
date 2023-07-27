.DEFAULT_GOAL := util

.PHONY: util
util: sm3pump sm3 ccode

sm3pump: sm3pump.c
	gcc -I../include -L../lib -o ../bin/sm3pump sm3pump.c -lmyy

sm3: sm3_util.c
	gcc -I../include -L../lib -o ../bin/sm3 sm3_util.c -lmyy

ccode: ccode.go
	go build -o ../bin/go ccode.go