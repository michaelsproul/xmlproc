CC= gcc
CFLAGS=-Wall -W -pedantic -std=c99 -g
LDFLAGS=-lm

all: solarxml

solarxml: solarxml.c
	$(CC) $(CFLAGS) -o $@ solarxml.c $(LDFLAGS)

debug: solarxml.c
	$(CC) $(CFLAGS) -D_DEBUG -o solarxml solarxml.c $(LDFLAGS)

clean:
	rm solarxml
