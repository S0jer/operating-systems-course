//
// Created by ppjas on 5/29/2022.
//

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>


#define REINDEER_NUMBER 9
#define ELVES_NUMBER 10
#define ELVES_WAITING_FOR_SANTA 3
#define ELF_WORK_TIME rand() % 4 + 2
#define SANTA_SOLVE rand() % 2 + 1
#define HOLIDAY_REINDEER rand() % 6 + 2
#define DELIVERING_T rand() % 3 + 1

int reindeerBack = 0;
int deliveries = 0;
int elvesWaitingForSanta = 0;
int elvesQueue[ELVES_WAITING_FOR_SANTA];
bool reindeerWaiting = false;

pthread_mutex_t mutexS = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condS = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexR = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexRWait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condRWait = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexE = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexEWait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condEWait = PTHREAD_COND_INITIALIZER;


void resetQueue(int *queue) {
    for (int i = 0; i < ELVES_WAITING_FOR_SANTA; i++) {
        queue[i] = -1;
    }
}


void *santa(void *arg) {
    while (true) {
        pthread_mutex_lock(&mutexS);
        while (elvesWaitingForSanta < ELVES_WAITING_FOR_SANTA && reindeerBack < REINDEER_NUMBER) {
            printf("Mikołaj śpi ; elfy: (%d) ; renifery (%d)\n", elvesWaitingForSanta, reindeerBack);
            pthread_cond_wait(&condS, &mutexS);
        }
        pthread_mutex_unlock(&mutexS);
        printf("Mikołaj budzi się ; elfy: (%d) ; renifery (%d)\n", elvesWaitingForSanta, reindeerBack);
        pthread_mutex_lock(&mutexR);

        if (reindeerBack == REINDEER_NUMBER) {
            deliveries++;
            printf("Mikołaj dostarcza zabawki :D [delivery no: %d]\n", deliveries);
            sleep(DELIVERING_T);
            reindeerBack = 0;

            pthread_mutex_lock(&mutexRWait);
            reindeerWaiting = false;
            pthread_cond_broadcast(&condRWait);
            pthread_mutex_unlock(&mutexRWait);
        }
        pthread_mutex_unlock(&mutexR);
        if (deliveries == 3) break;
        pthread_mutex_lock(&mutexE);

        if (elvesWaitingForSanta == ELVES_WAITING_FOR_SANTA) {
            printf("Mikołaj rozwiązuje problemy elfów :O [%d][%d][%d]\n", elvesQueue[0], elvesQueue[1], elvesQueue[2]);
            sleep(SANTA_SOLVE);
            pthread_mutex_lock(&mutexEWait);
            resetQueue(elvesQueue);
            elvesWaitingForSanta = 0;
            pthread_cond_broadcast(&condEWait);
            pthread_mutex_unlock(&mutexEWait);
        }
        printf("Mikołaj: zasypiam\n\n");
        pthread_mutex_unlock(&mutexE);
    }
    exit(0);
}


void *elf(void *arg) {
    int ID = *((int *) arg);
    while (true) {
        sleep(ELF_WORK_TIME);
        pthread_mutex_lock(&mutexEWait);

        while (elvesWaitingForSanta == ELVES_WAITING_FOR_SANTA) {
            printf("Czekamy na powrót elfów ; ID: %d\n", ID);
            pthread_cond_wait(&condEWait, &mutexEWait);
        }
        pthread_mutex_unlock(&mutexEWait);
        pthread_mutex_lock(&mutexE);

        if (elvesWaitingForSanta < ELVES_WAITING_FOR_SANTA) {
            elvesQueue[elvesWaitingForSanta] = ID;
            elvesWaitingForSanta++;
            printf("Na Mikołaja czeka [%d] elfów ; ID: %d\n", elvesWaitingForSanta, ID);
            if (elvesWaitingForSanta == ELVES_WAITING_FOR_SANTA) {
                printf("Elf wybudza Mikołaja ; ID: %d\n", ID);
                pthread_mutex_lock(&mutexS);
                pthread_cond_broadcast(&condS);
                pthread_mutex_unlock(&mutexS);
            }
        }
        pthread_mutex_lock(&mutexEWait);
        pthread_cond_wait(&condEWait, &mutexEWait);
        pthread_mutex_unlock(&mutexEWait);
    }
}


_Noreturn void *reindeer(void *arg) {
    int ID = *((int *) arg);
    while (true) {
        pthread_mutex_lock(&mutexRWait);
        while (reindeerWaiting) {
            pthread_cond_wait(&condRWait, &mutexRWait);
        }
        pthread_mutex_unlock(&mutexRWait);

        sleep(HOLIDAY_REINDEER);

        pthread_mutex_lock(&mutexR);
        reindeerBack++;
        printf("Na Mikołaja czeka [%d] reniferów ; %d\n", reindeerBack, ID);
        reindeerWaiting = true;
        if (reindeerBack == REINDEER_NUMBER) {
            printf("Renifer wybudza Mikołaja ; %d\n", ID);
            pthread_mutex_lock(&mutexS);
            pthread_cond_broadcast(&condS);
            pthread_mutex_unlock(&mutexS);
        }
        pthread_mutex_unlock(&mutexR);
    }
}


int main() {
    srand(time(NULL));

    resetQueue(elvesQueue);
    pthread_t santa_t;
    pthread_create(&santa_t, NULL, &santa, NULL);

    int *reindeer_IDs = calloc(REINDEER_NUMBER, sizeof(int));
    pthread_t *reindeer_t = calloc(REINDEER_NUMBER, sizeof(pthread_t));
    for (int i = 0; i < REINDEER_NUMBER; i++) {
        reindeer_IDs[i] = i;
        pthread_create(&reindeer_t[i], NULL, &reindeer, &reindeer_IDs[i]);
    }

    int *elves_IDs = calloc(ELVES_NUMBER, sizeof(int));
    pthread_t *elves_t = calloc(ELVES_NUMBER, sizeof(pthread_t));
    for (int i = 0; i < ELVES_NUMBER; i++) {
        elves_IDs[i] = i;
        pthread_create(&elves_t[i], NULL, &elf, &elves_IDs[i]);
    }

    pthread_join(santa_t, NULL);
    for (int i = 0; i < REINDEER_NUMBER; i++) {
        pthread_join(reindeer_t[i], NULL);
    }

    for (int i = 0; i < ELVES_NUMBER; i++) {
        pthread_join(elves_t[i], NULL);
    }
    return 0;
}
