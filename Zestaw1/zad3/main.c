#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myLibrary.h"
#include <sys/times.h>
#include <unistd.h>
#include <dlfcn.h>


long double time_sec(clock_t time) {
    return (long double) (time) / sysconf(_SC_CLK_TCK);
}

void printTime(clock_t clock_start, clock_t clock_end, struct tms start, struct tms end) {
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end.tms_utime - start.tms_utime));
    printf("sys %Lf\n\n", time_sec(end.tms_stime - start.tms_stime));
}


int main(int argc, char *argv[]) {

    struct tms start;
    struct tms end;
    clock_t clock_start;
    clock_t clock_end;

#ifdef DyLIBRARY
    void *handle = dlopen("./libmyLibrary.so", RTLD_LAZY);
    if (!handle) {/*error*/}
    struct Array*(*createArray)(int) = (struct Array*(*)(int)) dlsym(handle, "createArray");
    void (*getData)(char *) = (void (*)(char *))  dlsym(handle, "getData");
    int (*onBlockOperations)(struct Array *) = (int(*)(struct Array *)) dlsym(handle, "onBlockOperations");
    void (*deleteBlock)(struct Array *, int) = (void (*)(struct Array *, int)) dlsym(handle, "deleteBlock");
#endif

    struct Array *arr = NULL;

    clock_start = times(&start);
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "createArray") == 0) {
            int size = atoi(argv[++i]);

            arr = createArray(size);
        } else if (strcmp(argv[i], "getData") == 0) {
            int j = i + atoi(argv[++i]) + 1;
            i += 1;
            for (i; i < j; ++i) {
                getData(argv[i]);
                onBlockOperations(arr);
            }
            i -= 1;
        } else if (strcmp(argv[i], "deleteBlock") == 0) {
            int idx = atoi(argv[++i]);
            deleteBlock(arr, idx);
        }


    }
    clock_end = times(&end);
    printf("TOTAL TIME\n");
    printTime(clock_start, clock_end, start, end);


    return 0;
}



