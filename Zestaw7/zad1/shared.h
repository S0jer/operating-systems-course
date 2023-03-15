//
// Created by ppjas on 5/10/2022.
//

#ifndef SYSOPY_SHARED_H
#define SYSOPY_SHARED_H

#include <stdlib.h>
#include <pwd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define ID 'R'
#define OVEN_ID 'O'
#define TABLE_ID 'T'

#define OVEN_PATH "oven"
#define TABLE_PATH "table"

#define OVEN_SIZE 5
#define TABLE_SIZE 5

#define OVEN_SEM 0
#define TABLE_SEM 1
#define FULL_OVEN_SEM 2
#define FULL_TABLE_SEM 3
#define EMPTY_TABLE_SEM 4

#define PREPARATION_TIME 1
#define BAKING_TIME 4
#define DELIVERY_TIME 4
#define RETURN_TIME 4


char *homePath() {
    char *path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}


typedef struct {
    int arr[OVEN_SIZE];
    int pizzas;
    int placeIdx;
    int takeOutIdx;
} oven;


typedef struct {
    int arr[TABLE_SIZE];
    int pizzas;
    int placeIdx;
    int takeOutIdx;
} table;


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;

} arg;


void lockSem(int semId, int semNum) {
    struct sembuf s = {.sem_num = semNum, .sem_op = -1};
    if (semop(semId, &s, 1) == -1) {
        printf("Locking semaphore error \n");
        exit(1);
    }
}


void unlockSem(int semId, int semNum) {
    struct sembuf s = {.sem_num = semNum, .sem_op = 1};
    if (semop(semId, &s, 1) == -1) {
        printf("Unlocking semaphore error \n");
        exit(1);
    }
}


int getSemId() {
    key_t key = ftok(homePath(), ID);
    int semId;
    if ((semId = semget(key, 5, 0)) == -1) {
        printf("Connecting to semaphore set error! \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return semId;
}


int getShmOvenId() {
    key_t key_o = ftok(OVEN_PATH, OVEN_ID);
    int shmOvenId;
    if ((shmOvenId = shmget(key_o, sizeof(oven), 0)) == -1) {
        printf("Getting oven ID error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
    return shmOvenId;
}


int getShmTableId() {
    key_t key_t = ftok(TABLE_PATH, TABLE_ID);
    int shmTableId;
    if ((shmTableId = shmget(key_t, sizeof(table), 0)) == -1) {
        printf("Getting table ID error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
    return shmTableId;
}


void printArr(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


char *getCurrentTime() {
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    int milliseconds = curTime.tv_usec / 1000;

    char *buffer = calloc(80, sizeof(char));
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

    char *currentTime = calloc(84, sizeof(char));
    sprintf(currentTime, "%s:%03d", buffer, milliseconds);

    return currentTime;
}

#endif //SYSOPY_SHARED_H
