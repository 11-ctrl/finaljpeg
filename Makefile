# ==========================================
# Makefile for MMSP JPEG-like Project
# ==========================================

CC = gcc
CFLAGS = -Wall -O2

all: encoder decoder

encoder: encoder.c
	$(CC) $(CFLAGS) encoder.c -o encoder -lm

decoder: decoder.c
	$(CC) $(CFLAGS) decoder.c -o decoder -lm

clean:
	rm -f encoder decoder *.o Res*.bmp

.PHONY: all clean
