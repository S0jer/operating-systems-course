//
// Created by ppjas on 3/22/2022.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/times.h>


long double time_sec(clock_t time) {
    return (long double) (time) / sysconf(_SC_CLK_TCK);
}


void printTime(clock_t clock_start, clock_t clock_end, struct tms start, struct tms end) {
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end.tms_utime - start.tms_utime));
    printf("sys %Lf\n\n", time_sec(end.tms_stime - start.tms_stime));
}


float countRectangleArea(float start, float width) {
    float x = start + (width / 2);
    float height;

    height = 4 / (x * x + 1);

    return height * width;
}


int max(int a, int b) {
    if (a >= b) return a;
    return b;
}


int main(int argc, char *argv[]) {

    struct tms start;
    struct tms end;
    clock_t clock_start;
    clock_t clock_end;
    clock_start = times(&start);


    if (argc != 3) {
        printf("Wrong number of arguments :)");
        exit(1);
    }

    float width = atof(argv[1]);
    int n = atoi(argv[2]);
    int i = 0;
    int calculations = 1 / width;
    int forProcces = max(calculations / n, 1);
    float begin = 0;

    pid_t pid;

    while (i < calculations / forProcces) {
        pid = fork();
        if (pid == 0) {
            // printf("Process: %d  Child PID: %d  Parent PID: %d\n", i + 1, getpid(), getppid());
            int add = 0;
            char *fileName = (char *) calloc(1, 256);
            char *number = (char *) calloc(1, 256);
            sprintf(number, "%d", i + 1);
            strcat(fileName, "w");
            strcat(fileName, number);
            strcat(fileName, ".txt");
            FILE *f = fopen(fileName, "w+");
            if (f == NULL) {
                perror("Cannot open file :)");
                exit(1);
            }
            for (int j = 0; j <= forProcces; j++) {
                if (begin + width <= 1) {
                    fprintf(f, "%f\n", countRectangleArea(begin, width));
                    begin += width;
                }
            }
            free(fileName);
            free(number);
            fclose(f);
            return 0;
        }
        begin += width * (forProcces + 1);
        i += 1;
    }
    for (int k = 0; k < n; k++) wait(NULL);


    float result = 0;
    for (int l = 0; l < calculations / forProcces; l++) {
        char *fileName = (char *) calloc(1, 256);
        char *number = (char *) calloc(1, 256);
        sprintf(number, "%d", l + 1);
        strcat(fileName, "w");
        strcat(fileName, number);
        strcat(fileName, ".txt");
        FILE *f = fopen(fileName, "r");

        char *add = (char *) calloc(1, 256);
        while (fgets(add, 256, f)) {
            result += atof(add);
        }

        free(fileName);
        free(number);
        free(add);
        fclose(f);
    }


    clock_end = times(&end);
    printf("\n ######### Time #########\n");
    printTime(clock_start, clock_end, start, end);

    printf("\n Result: %f\n", result);

}