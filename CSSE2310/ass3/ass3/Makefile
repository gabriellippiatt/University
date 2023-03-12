CFLAGS= -Wall -pedantic -std=gnu99

.PHONY: all
.DEFAULT_GOAL := all

all: 2310A 2310B 2310dealer

2310A: playerA.c
	gcc  $(CFLAGS) playerA.c players.c -o 2310A
2310B: playerB.c
	gcc  $(CFLAGS) playerB.c players.c -o 2310B
2310dealer: dealer.c
	gcc  $(CFLAGS) dealer.c -o 2310dealer
