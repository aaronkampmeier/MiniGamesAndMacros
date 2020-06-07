//
// Created by Aaron Kampmeier on 6/3/20.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>
#include <stdlib.h>

int main() {
    printf("Starting memory leaks\n");
    int arrayLen = 10000;
    long int memoryUsed = 0;
    int i;

//    for (i=0; i < 1000000000; i++) {
//        printf("Making leak %d\n", i);
//
//        //Make leak
//        double* myArray = (double*) malloc(arrayLen * sizeof(double));
//        for (int j=0; j < arrayLen; j++) {
//            *(myArray + j) = 3.14 * j;
//        }
//        memoryUsed += arrayLen * sizeof(double);
//        if (i % 10000000 == 0) {
//            printf("Used %ld bytes of memory on leak %d\n", memoryUsed, i);
//        }
//
//        free(myArray);
//    }

// Corrupt some memory
    double* myArray = (double*) malloc(10000000 * sizeof(int));
    myArray[0] = 2;
    for (int k=0; k < 10000000000000000000; k++) {
//        printf("%d: %d\n", i, *(myArray + i));
        *(myArray + k) = 5;
//        printf("%d: %d\n", i, *(myArray + i));
//        myArray[i] = 5;
    }

//    free(myArray);
}
