//
// Created by ppjas on 3/15/2022.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "myLibrary.h"


struct Array *createArray(int size) {
    struct Array *arr = calloc(1, sizeof(struct Array));

    arr->size = size;
    arr->lastIndex = -1;
    arr->blocks = calloc(size, sizeof(struct Array));
    printf("Created array of size %d\n\n", size);

    return arr;
}


void getData(char *fileName) {
    FILE *opFile = fopen(fileName, "r");
    if (opFile == NULL) {
        perror("Cannot open file :)");
        exit(1);
    }


    char *f = (char *) calloc(1, 256);

    strcat(f, "wc <");
    strcat(f, fileName);
    strcat(f, " > tmpFile.txt");
    system(f);
    printf("%s", f);
    free(f);

}


int onBlockOperations(struct Array *mArray) {
    FILE *tmpFile = fopen("tmpFile.txt", "r");
    if (tmpFile == NULL) {
        printf("Błąd tworzenia pliku.");
        exit(1);
    }

    struct MemoryBlock *block = calloc(1, sizeof(struct MemoryBlock));
    block->data = (char *) calloc(1, 256);

    fgets(block->data, 256, tmpFile);

    mArray->blocks[mArray->lastIndex + 1] = block;
    mArray->lastIndex++;
    fclose(tmpFile);
    system("rm tmpFile.txt");

    return mArray->lastIndex;
}


void deleteBlock(struct Array *arr, int id) {
    if (arr->blocks[id] == NULL) return;
    free(arr->blocks[id]->data);
    free(arr->blocks[id]);
    arr->blocks[id] = NULL;
}


void printArray(struct Array *arr) {
    struct MemoryBlock *block;
    for (int i = 0; i < arr->size; i++) {
        block = arr->blocks[i];
        if (block != NULL){
            printf("BLOCK NUMBER %d: ", i);
            printf("%s\n", block->data);
        }
    }
    printf("\n");
}