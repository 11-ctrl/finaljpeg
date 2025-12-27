CC = gcc
CFLAGS = -Wall -O2

ENC = encoder
DEC = decoder

all: $(ENC) $(DEC)

$(ENC): encoder.c
	$(CC) $(CFLAGS) encoder.c -o $(ENC)

$(DEC): decoder.c
	$(CC) $(CFLAGS) decoder.c -o $(DEC)

run: all
	./$(ENC) 0 Kimberly.bmp R.txt G.txt B.txt dim.txt
	./$(DEC) 0 ResKimberly.bmp R.txt G.txt B.txt dim.txt
	@echo "Mode 0 encode/decode finished."

clean:
	rm -f $(ENC) $(DEC) R.txt G.txt B.txt dim.txt ResKimberly.bmp
