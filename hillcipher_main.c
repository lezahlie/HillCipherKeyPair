/*
    Author: Leslie Horace
    File: hillcipher_main.c
    Purpose: c program to demonstrate functions provided by hillcipher_keypair.h
*/

#include <stdlib.h>     // c standard library
#include <stdio.h>      // writing and reading files
#include <string.h>     // i/o file names
#include <time.h>       // for seeding randomness
#include "hillcipher_keypair.h"


/* main entry point for generating hill cipher encryption/decryption keys  */
int main(int argc, char **argv){

    if(argc != 3){
        printf("usage: %s <enc key filename> <dec key filename>\n", (char*)argv[0]);
        exit(EXIT_FAILURE);
    }
    char * ekey_fn = argv[1];
    char * dkey_fn = argv[2];

    srand(time(NULL));      // time seed for randomness

    printf("\nGenerating encryption/decryption key pairs...\n");
    if(createKeyPair(ekey_fn, dkey_fn) == 1){
        exit(EXIT_FAILURE);
    }

    int * ekey, * dkey, n;  // key matrices and matrix order

    printf("\nReading key files '%s' and '%s'...\n", ekey_fn, dkey_fn);

    if(readKey(ekey_fn, &ekey, &n) != 0){
        exit(EXIT_FAILURE);
    }
    printf("\nEncryption Key ['%s']:\n", ekey_fn);
    printMatrix(ekey, n, n);
    free(ekey);

    if(readKey(dkey_fn, &dkey, &n) != 0){
        exit(EXIT_FAILURE);
    }
    printf("\nDecryption Key ['%s']:\n", dkey_fn);
    printMatrix(dkey, n, n);
    free(dkey);

    exit(EXIT_SUCCESS);
}
