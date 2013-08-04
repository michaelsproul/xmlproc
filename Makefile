CC= gcc
CFLAGS=-Wall -W -pedantic -std=c99 -g
LDFLAGS=-lm

all: xmlproc

xmlproc: xmlproc.c
	$(CC) $(CFLAGS) -o $@ xmlproc.c $(LDFLAGS)

debug: xmlproc.c
	$(CC) $(CFLAGS) -D_DEBUG -o xmlproc xmlproc.c $(LDFLAGS)

clean:
	rm xmlproc
