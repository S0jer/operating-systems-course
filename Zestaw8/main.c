//
// Created by ppjas on 5/16/2022.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#define ROW_LENGTH 256

int **img;
int **negImg;
int w, h;
int threadsNum;


void saveNeg(char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error while opening output file\n");
        exit(1);
    }

    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", w, h);
    fprintf(f, "255\n");

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            fprintf(f, "%d ", negImg[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}


void *numMethod(void *arg) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    int idx = *((int *) arg);
    int from = 256 / threadsNum * idx;
    int to = (idx != threadsNum - 1) ? (256 / threadsNum * (idx + 1)) : 256;
    int p;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            p = img[i][j];
            if ((p >= from && p < to)) {
                negImg[i][j] = 255 - p;
            }
        }
    }
    gettimeofday(&stop, NULL);
    long unsigned int *t = malloc(sizeof(long unsigned int));
    *t = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(t);
}


void loadImg(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Opening input file error \n");
        exit(1);
    }

    char *buf = calloc(ROW_LENGTH, sizeof(char));
    fgets(buf, ROW_LENGTH, f);
    bool getM;
    bool rImg = false;

    while (!rImg && fgets(buf, ROW_LENGTH, f)) {
        if (buf[0] == '#') {
            continue;
        } else if (!getM) {
            sscanf(buf, "%d %d\n", &w, &h);
            printf("w: %d, h: %d\n", w, h);
            getM = true;
        } else {
            int M;
            sscanf(buf, "%d \n", &M);
            printf("M: %d\n", M);
            if (M != 255) {
                printf("Max grey value too low! \n");
                exit(1);
            }
            rImg = true;
        }

        img = calloc(h, sizeof(int *));
        for (int i = 0; i < h; i++) {
            img[i] = calloc(w, sizeof(int));
        }

        int p;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                fscanf(f, "%d", &p);
                img[i][j] = p;
            }
        }
        fclose(f);
    }
}


void *blockMethod(void *arg) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    int idx = *((int *) arg);
    int xFrom = (idx) * ceil(w / threadsNum);
    int xTo = (idx != threadsNum - 1) ? ((idx + 1) * ceil(w / threadsNum) - 1) : w - 1;
    int p;

    for (int i = 0; i < h; i++) {
        for (int j = xFrom; j <= xTo; j++) {
            p = img[i][j];
            negImg[i][j] = 255 - p;
        }
    }
    gettimeofday(&stop, NULL);
    long unsigned int *t = malloc(sizeof(long unsigned int));
    *t = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(t);
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Wrong number of arguments :) \n");
        exit(1);
    }

    threadsNum = atoi(argv[1]);
    char *method = argv[2];
    char *inputFile = argv[3];
    char *outputFile = argv[4];

    if ((strcmp(method, "numbers") != 0) && (strcmp(method, "block") != 0)) {
        printf("Wrong method :( \n");
        exit(1);
    }

    loadImg(inputFile);
    negImg = calloc(h, sizeof(int *));
    for (int i = 0; i < h; i++) {
        negImg[i] = calloc(w, sizeof(int));
    }

    pthread_t *threads = calloc(threadsNum, sizeof(pthread_t));
    int *threadsIdx = calloc(threadsNum, sizeof(int));
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    for (int i = 0; i < threadsNum; i++) {
        threadsIdx[i] = i;
        if (strcmp(method, "numbers") == 0) {
            pthread_create(&threads[i], NULL, &numMethod, &threadsIdx[i]);
        } else if (strcmp(method, "block") == 0) {
            pthread_create(&threads[i], NULL, &blockMethod, &threadsIdx[i]);
        }
    }

    FILE *time = fopen("times.txt", "a");
    printf("THREADS:   %d\n", threadsNum);
    printf("METHOD:    %s\n", method);
    fprintf(time, "\nTHREADS:   %d\n", threadsNum);
    fprintf(time, "METHOD:    %s\n\n\n", method);

    for (int i = 0; i < threadsNum; i++) {
        long unsigned int *t;
        pthread_join(threads[i], (void **) &t);
        printf("Thread: %3d     Time: %5lu [μs]\n", i, *t);
        fprintf(time, "Thread: %3d,     Time: %5lu [μs]\n", i, *t);
    }

    gettimeofday(&stop, NULL);
    long unsigned int *t = malloc(sizeof(long unsigned int));
    *t = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("TOTAL TIME: %5lu [μs]\n", *t);
    fprintf(time, "TOTAL TIME: %5lu [μs]\n\n\n", *t);

    saveNeg(outputFile);
    fclose(time);

    for (int i = 0; i < h; i++) {
        free(img[i]);
        free(negImg[i]);
    }
    free(img);
    free(negImg);

    return 0;
}
