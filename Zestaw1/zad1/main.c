#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "myLibrary.h"


int main(int argc, char *argv[]) {
    struct Array *arr;
    arr = createArray(argc - 1);

    for(int i = 1; i < argc; i++){
        getData(argv[i]);
        onBlockOperations(arr);
    }

    printArray(arr);
    deleteBlock(arr, 0);
    printArray(arr);
    return 0;
}



