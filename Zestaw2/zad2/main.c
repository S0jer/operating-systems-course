//
// Created by ppjas on 3/17/2022.
//

//
// Created by ppjas on 3/17/2022.
//


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>


long double time_sec(clock_t time) {
    return (long double) (time) / sysconf(_SC_CLK_TCK);
}

void printTime(clock_t clock_start, clock_t clock_end, struct tms start, struct tms end) {
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end.tms_utime - start.tms_utime));
    printf("sys %Lf\n\n", time_sec(end.tms_stime - start.tms_stime));
}



void copyLib(char character, char *file) {
    FILE *f1 = fopen(file, "r");

    if (f1 == NULL) {
        perror("Can not open a file.");
        exit(1);
    }

    char buffer;
    int copy = 0;
    int charCounter = 0;
    int lineCounter = 0;
    int was = false;

    while (true) {
        fread(&buffer, sizeof(char), 1, f1);
        if (buffer == character) {
            charCounter++;
            was = true;
        }
        if ((buffer == '\n' && copy == 1)) {
            if (was) lineCounter++;
            was = false;
        }
        if (!isspace(buffer)) {
            copy = 1;
        }
        if (feof(f1)) break;
    }
    if (copy == 1) {
        if (was) lineCounter++;
        was = false;
    }
    printf("copyLib: %d %d\n", charCounter, lineCounter);

    fclose(f1);
}


void copySys(char character, char *file) {
    int f1 = open(file, O_RDONLY);

    if (f1 < 0) {
        perror("Can not open a file.");
        exit(1);
    }

    char buffer;
    int copy = 0;
    int charCounter = 0;
    int lineCounter = 0;
    int was = false;

    while (true) {

        if (read(f1, &buffer, sizeof(char)) == 0) break;
        if (buffer == character) {
            charCounter++;
            was = true;
        }
        if (!isspace(buffer)) {
            copy = 1;
        }
        if ((buffer == '\n' && copy == 1)) {
            if (was) {
                lineCounter++;
                was = false;
            }
            copy = 0;
        }

    }
    if (copy == 1) {
        if (was) {
            lineCounter++;
            was = false;
        }
    }
    printf("copySys: %d %d\n", charCounter, lineCounter);

    close(f1);
}


int main(int argc, char *argv[]) {


    struct tms startSys; struct tms endSys;
    struct tms startLib; struct tms endLib;
    clock_t clock_startLib; clock_t clock_endLib;
    clock_t clock_startSys; clock_t clock_endSys;
    char character;
    char *file;


    character = *(argv[1]);
    file = argv[2];

    printf("\n Sys Time \n");
    clock_startLib = times(&startLib);
    copyLib(character, file);
    clock_endLib = times(&endLib);
    printTime(clock_startLib, clock_endLib, startLib, endLib);

    printf("\n Lib Time \n");
    clock_startSys = times(&startSys);
    copySys(character, file);
    clock_endSys = times(&endSys);
    printTime(clock_startSys, clock_endSys, startSys, endSys);

}


