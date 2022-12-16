/*
    Author: Leslie Horace
    File: hillcipher_keypair.h
    Purpose: header file to call functions necessary for creating and utilizing encyrption/decryption key pairs
*/

#ifndef HILLCIPHER_KEYPAIR_
#define HILLCIPHER_KEYPAIR_

/* 
    allocates space for a 1d array of n*m size 
    returns a (int *) pointer to the array
*/
int *malloc1D(int m, int n);

/* 
    reads a binary file key into a matrix
    params: input filename.bin, M = pointer to matrix, n = matrix order
    returns 1 (error) or 0 (success)
*/
int readKey(char * oFile, int **M, int *n);


/* 
    prints a m*n matrix to the console
    params: matrix pointer X, m = row size, n = col size
*/
void printMatrix(int *X, int m, int n);

/*
    calculates GCD(a,b) = d, where ax + by = d
    params: a = integer, b = modulus, x = a inverse, and y = b inverse
    returns d, inverse x and y are returned as arguments
*/
int extEuclid(int a, int b, int *x, int *y);

/* 
    computes positive remainder = a (mod 26)
    returns the remainder
*/
int postiveMod(int a);

/*  
    generates a random matrix order n from [2,9]
    creates random n*n encyrption and decyrption key matrices
    writes out both keys to their specified file names
    params: encryption key file name, decyrption key file name
    returns 1 (error) or 0 (success)
*/
int createKeyPair(char * ekey_fname, char * dkey_fname);


#endif /* MATRIX_KEY_ */