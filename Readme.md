```
Author: Leslie Horace
Purpose: Dynamically finding hill cipher encryption/decryption key pairs 
Version: 1.0
```
# Hill Cipher KeyPair Generator

## Included files
- hillcipher_main.c
    - Functions for creating encyrption key and computing decyrption key
- hillcipher_main.h
    - Header file to use functions from "hillcipher_main.c"
- hillcipher_main.c
    - simple main to demonstrate hill cipher key pair generation
- Makefile 
    - compiling program and cleaning out files

##  Compiling the program
> make

> gcc ./hillcipher_main.c ./hillcipher_keypair.c -o < outfile>

##  Cleaning compiled files
> make clean

## Running the program
usage: < outfile> <enc_key filename> <dec_key filename>

*output files are written in binary, use (.bin) extension*

##  About the program
1. Creates a random square key matrix of order n, where n is in range [2, 9]
    + The matrix is filled with random codes [0,26] for language [A-Z]
    + Keeping the order small for demoing purposes
2.  Computes the modular inverse by performing elementary row operations
    + Row Multiplication
    + Row swap
    + Row addition 
3.  All compuations are reduced (mod 26) for language [A-Z]
    + [MODULUS] macro in "hillcipher_key.c" can be modified as needed
4.  Generates new encryption keys untill finding one that is invertable
5.  Upon finding valid keypair, each key is written to the filename arguments
6.  The keys can be read back in for use and/or display to the console

