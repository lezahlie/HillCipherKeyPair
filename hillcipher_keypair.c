/*
    Author: Leslie Horace
    File: hillcipher_keypair.h
    Purpose: c program to generate random hill cipher encryption/decryption key pair
*/

#include <stdlib.h>     // c standard library
#include <stdio.h>      // writing and reading files
#include <string.h>     // i/o file names
#include "hillcipher_keypair.h"
#define MODULUS 26      // hill cipher is invertable mod 26 for lanaguge [A-Z]

// allocates 1dim space for any m*n matrix
int *malloc1D(int m, int n) {
    int *X = (int*)malloc(m*n*sizeof(int));
    for(int i = 0; i < m*n; i++)
        X[i] = 0;       // initialize each index
    return X;       // return pointer to matrix
}

//  reads hill cipher key from a file into a n*n matrix
int readKey(char * oFile, int **M, int *n){
    FILE * fp = NULL; 
    // check if file is open for reading
    if ((fp = fopen(oFile, "rb")) != NULL) {
        fread(&(*n), sizeof(int), 1, fp);       // read matrix order
        *M = malloc1D((*n), (*n));
        fread(M[0], sizeof(int), (*n)*(*n), fp);        // read matrix contents
        fclose(fp); 
    }else{  // here if file read error occured
        perror("File read error...");
        return 1;
    }
    return 0;
}

//  writes hill cipher key from a n*n matrix to a file
int writeKey(char * oFile, int *M, int n){
    FILE * fp = NULL; 
    // check if file is open for writing
    if ((fp = fopen(oFile, "wb")) != NULL){ 
        fwrite(&n, sizeof(int), 1, fp);         // write matrix order
        fwrite(M, sizeof(int), n*n, fp);        // write matrix contents
        fclose(fp);
    }else{      // here if file write error occured
        perror("File write error...");
        return 1;
    }
    return 0;
}

// prints any n*m matrix
void printMatrix(int *X, int m, int n){
    printf("\n");
    for(int i =0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            printf("%d\t", X[i*n+j]);
        }printf("\n");
    }
    printf("\n");
}

// performs the extended euclidian algorithm
int extEuclid(int a, int b, int *x, int *y){   
    int d = 0;
    if(a == 0){         // error case: a,b are not relatively prime
        *x=a;  
        *y=a+1;
        d = b;  
    }else{      // recursive case: a,b are relatively prime
        int x_tmp = 0, y_tmp = 0;
        // backwards substitution to find inverses s.t. ax+by=1
        d = extEuclid(b%a, a, &x_tmp, &y_tmp);
        *x = y_tmp - (x_tmp * (b/a));
        *y = x_tmp; 
    }
    return d;   // return the GCD
}

// calculates and returns the positive remainder of mod 26
int positiveMod(int a) {
    int r = a % MODULUS;   
    if (r < 0){    
        r += MODULUS;   
    }
    return r; 
}

// initializes K = key matrix and A = augmented matrix [K | I], where I = identity
void initKeys(int * K, int * A, int n, int m){
    for(int i =0; i < n; i ++){
        for(int j =0; j < n; j++){
            K[i*n+j] = A[i*m+j] = rand() % 26;
            // set RIGHT half of A[K | I] as identity matrix
            if(i==j){ 
                A[i*m+(j+n)] = 1;       // diagonal 1
            }else{
                A[i*m+(j+n)] = 0;       // all else 0
            }
        }
    }
}

// row operation: swaps the pivots row with the target row
void swapRow(int *A, int target, int pivot,int m){
    int x=0;
    for(int j=0; j < m; j++){
        x = A[pivot*m+j];       // temp = A[pivot][j]
        A[pivot*m+j] = A[target*m+j];       // A[pivot][j] = A[target][j]
        A[target*m+j] = x;      // A[target][j] = temp
    }
}

// row operation: scale the row by multiplying with the pivots modular inverse
int scaleRow(int *A, int pivot, int n, int m){
    int x = 0, y = 0;
    // loops starts with the pivot's row 
    for (int i=pivot; i < n; i++){
        // check if current index A[i][pivot] is invertable (mod 26)
        if(extEuclid(A[i*m+pivot], MODULUS, &x, &y) == 1){
            // if current row is not the pivot row, then swap rows
            if(i != pivot){     
                swapRow(A, i, pivot, m);
            }
            // multiple the row by the pivot's inverse
            for(int j =0; j < m; j++){
                A[pivot*m+j] *= x;              // A[pivot][j] = A[pivot][pivot]^-1 * A[pivot][j]
                A[pivot*m+j] = positiveMod(A[pivot*m+j]);   // reduce by (mod 26)
            }
            return 1;   // return 1(true) = is invertable mod 26
        }
    }
    return 0;   // return 0 (false) = not invertable mod 26
}

// row operation: subtract each row index by a multiple of pivots row index
void subtractRow(int *A, int pivot, int n, int m){
    int x = 0;
    for (int i=0; i < n; i++){
        // check if the current row is not the pivot row
		if(i != pivot){ 
            x = A[i*m+pivot];       // x = A[i][pivot]
			for(int j=0; j < m; j++){   
                A[i*m+j] -= (x * A[pivot*m+j]);         // A[i][j] = A[i][j] - (A[i][pivot] * A[pivot][j])
                A[i*m+j] = positiveMod(A[i*m+j]);       // reduce by (mod 26)
			}
		}
	}
}

// creates a random key and computes it inverse, continues until invertable key pair is found
void findInvertableKey(int * K, int * A, int n, int m){
    int valid = 0;
    do{
        initKeys(K, A, n, m);   // intilize a new key matrix and corresponding augmented matrix
        for(int i=0; i < n; i++){   
            // check if scaleRow function returns 1(true) for success
            if(scaleRow(A, i, n, m) == 1){  
                // perform row subtraction to tranform A[K | I] => A[I | K^-1]
                subtractRow(A, i, n, m);  
                valid = 1;
            } else{  // here if the key is not invertable, stop inner loop
                valid = 0;  
                break;
            }
        }
    }while(valid == 0); // loops until invertable key is found
}

// set new matrix X = K^-1 (from right half of transformed A[I|K^-1]) for writing
void getInverseKey(int * X,  int * A, int n, int m){
    for(int i =0; i < n; i++){
        for(int j =0; j < n; j++){
            X[i*n+j] = A[i*m+(j+n)];    // X[i][j] = I[i][j] in A[K|I]
        }
    }
}

// primary function for creating a new hill cipher key pair
int createKeyPair(char * ekey_fname, char * dkey_fname){

    int *K, *A; // K = key matrix pointer, A = augmented matrix pointer
    int n = (rand() % 8) + 2, m = 2*n;  // n = random [2-9] matrix order, m = 2*n for augmented matrix traversals

    // allocate space for key and augmented matrices
    K = malloc1D(n, n); 
    A = malloc1D(n, m);

    // find a invertable key pair
    findInvertableKey(K, A, n, m);

    // write the encyrption key to specfied filename
    printf("\nWriting encryption key to file '%s'...\n", ekey_fname);
    int res = writeKey(ekey_fname, K, n);
    if(res == 0){   // if encyrption key was successfully written, get the inversekey
        getInverseKey(K, A, n, m);  
        // write the decryption key to specfied filename
        printf("\nWriting decryption key to file '%s'...\n", dkey_fname);   
        res = writeKey(dkey_fname, K, n);
    }

    // deallocate space for matrices
    free(A);
    free(K);
    return res; // return result for writing keys to files
}