//
// Created by ppjas on 3/15/2022.
//

#ifndef SYSOPY_MYLIBRARY_H
#define SYSOPY_MYLIBRARY_H


struct MemoryBlock {
    char *data;
};

struct Array {
    int size;
    int lastIndex;
    struct MemoryBlock **blocks;
};


struct Array *createArray(int size);

void getData(char *fileName);

int onBlockOperations(struct Array *mArray);

void deleteBlock(struct Array *arr, int id);

void printArray(struct Array *arr);

#endif //SYSOPY_MYLIBRARY_H
