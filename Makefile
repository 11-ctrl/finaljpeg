CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm

all: encoder decoder

encoder: encoder.c
	$(CC) $(CFLAGS) encoder.c -o encoder $(LDFLAGS)

decoder: decoder.c
	$(CC) $(CFLAGS) decoder.c -o decoder $(LDFLAGS)

clean:
	rm -f encoder decoder *.o
