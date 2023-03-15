//
// Created by ppjas on 5/10/2022.
//


#include "shared.h"


int shmOvenId, shmTableId, semId;


void handler(int signum) {
    semctl(semId, 0, IPC_RMID, NULL);
    shmctl(shmOvenId, IPC_RMID, NULL);
    shmctl(shmTableId, IPC_RMID, NULL);
}


void setTable(table *table1) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table1->arr[i] = -1;
    }
    table1->pizzas = 0;
    table1->placeIdx = 0;
    table1->takeOutIdx = 0;
}


void setOven(oven *oven1) {
    for (int i = 0; i < OVEN_SIZE; i++) {
        oven1->arr[i] = -1;
    }
    oven1->pizzas = 0;
    oven1->placeIdx = 0;
    oven1->takeOutIdx = 0;
}


void createSemSet() {
    key_t key;

    if ((key = ftok(homePath(), ID)) == -1) {
        printf("Generating key error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }


    if ((semId = semget(key, 5, 0666 | IPC_CREAT)) == -1) {
        printf("Creating semaphore set error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    union arg;
    arg.val = 1;

    if (semctl(semId, OVEN_SEM, SETVAL, arg) == -1) {
        printf("Setting oven semaphore value error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    if (semctl(semId, TABLE_SEM, SETVAL, arg) == -1) {
        printf("Setting table semaphore value error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    arg.val = OVEN_SIZE;
    if (semctl(semId, FULL_OVEN_SEM, SETVAL, arg) == -1) {
        printf("Setting table semaphore value error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    arg.val = TABLE_SIZE;
    if (semctl(semId, FULL_TABLE_SEM, SETVAL, arg) == -1) {
        printf("Setting table semaphore value error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    arg.val = 0;
    if (semctl(semId, EMPTY_TABLE_SEM, SETVAL, arg) == -1) {
        printf("Setting table semaphore value error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    printf("Semaphore set created :) \nsemId: %d\n\n", semId);
}


void createShmSeg() {
    key_t key_o, key_t;

    if ((key_o = ftok(OVEN_PATH, OVEN_ID)) == -1) {
        printf("Generating key [oven] error \n");
        printf("%s", strerror(errno));
        exit(1);
    }

    if ((key_t = ftok(TABLE_PATH, TABLE_ID)) == -1) {
        printf("Generating key [table] error \n");
        printf("%s", strerror(errno));
        exit(1);
    }

    printf("o: %d, t: %d\n", key_o, key_t);
    if ((shmOvenId = shmget(key_o, sizeof(oven), IPC_CREAT | 0666)) == -1) {
        printf("Creating shared memory segment [oven] error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    if ((shmTableId = shmget(key_t, sizeof(table), IPC_CREAT | 0666)) == -1) {
        printf("Creating shared memory segment [table] error \n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    oven *oven1 = shmat(shmOvenId, NULL, 0);
    table *table1 = shmat(shmTableId, NULL, 0);

    setOven(oven1);
    setTable(table1);

    printf("Shared memory segment created :) \noven_id: %d, table_id: %d \n\n", shmOvenId, shmTableId);
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Wrong number of arguments :) \n");
        return -1;
    }

    signal(SIGINT, handler);

    int cook = atoi(argv[1]);
    int supp = atoi(argv[2]);

    createShmSeg();
    createSemSet();

    for (int i = 0; i < cook; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./cooking", "./cooking", NULL);
            printf("Return not expected... \n");
        }
    }

    for (int i = 0; i < supp; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./supp", "./supp", NULL);
            printf("Return not expected... \n");
        }
    }
    for (int i = 0; i < cook + supp; i++) {
        wait(NULL);
    }

    return 0;
}
