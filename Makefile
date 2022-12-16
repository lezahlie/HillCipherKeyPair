CC=gcc
CFLAGS=-Wall -g 

hill_keys: hillcipher_main.c  hillcipher_keypair.c hillcipher_keypair.h
	$(CC) $(CFLAGS)  hillcipher_main.c hillcipher_keypair.c -o hill_keys
clean:
	rm -f *.o *.bin hill_keys