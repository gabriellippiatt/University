CFLAGS= -pthread -Wall -pedantic -std=gnu99

.PHONY: all
.DEFAULT_GOAL := all

all: mapper2310 control2310 roc2310

mapper2310: mapper.c
	gcc  $(CFLAGS) mapper.c -o mapper2310
control2310: control.c
	gcc  $(CFLAGS) control.c -o control2310
roc2310: roc.c
	gcc  $(CFLAGS) roc.c -o roc2310