//
// Created by ppjas on 5/10/2022.
//

#include "shared.h"

void placePizzaOnTable(table *table1, int type) {
    table1->arr[table1->placeIdx] = type;
    table1->placeIdx++;
    table1->placeIdx = table1->placeIdx % OVEN_SIZE;
    table1->pizzas++;
}


int takeOutPizzaFromOven(oven *oven1) {
    int type = oven1->arr[oven1->takeOutIdx];
    oven1->arr[oven1->takeOutIdx] = -1;
    oven1->takeOutIdx++;
    oven1->takeOutIdx = oven1->takeOutIdx % OVEN_SIZE;
    oven1->pizzas--;
    return type;
}


void putToOven(oven *oven1, int type) {
    oven1->arr[oven1->placeIdx] = type;
    oven1->placeIdx++;
    oven1->placeIdx = oven1->placeIdx % OVEN_SIZE;
    oven1->pizzas++;
}


int main() {
    int semId = getSemId();
    int shmOvenId = getShmOvenId();
    int shmTableId = getShmTableId();
    oven *oven1 = shmat(shmOvenId, NULL, 0);
    table *table1 = shmat(shmTableId, NULL, 0);
    int type;

    srand(getpid());
    while (true) {
        // making pizza :O
        type = rand() % 10;
        printf("[Cook] PID: %d Time: %s  ->   Robie pizze: %d\n", getpid(), getCurrentTime(), type);
        sleep(PREPARATION_TIME);

        lockSem(semId, FULL_OVEN_SEM);
        // placing in oven
        lockSem(semId, OVEN_SEM);
        putToOven(oven1, type);
        printf("[Cook]  PID: %d Time: %s  ->   Dodałem pizze do pieca: %d. Liczba pizz w piecu: %d.\n", getpid(),
               getCurrentTime(), type, oven1->pizzas);
        unlockSem(semId, OVEN_SEM);
        // baking pizza
        sleep(BAKING_TIME);
        // take out pizza
        lockSem(semId, OVEN_SEM);
        type = takeOutPizzaFromOven(oven1);
        printf("[Cook] PID: %d Time: %s  ->   Wyjalem pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n",
               getpid(), getCurrentTime(), type, oven1->pizzas, table1->pizzas);
        unlockSem(semId, OVEN_SEM);

        unlockSem(semId, FULL_OVEN_SEM);
        lockSem(semId, FULL_TABLE_SEM);
        // place on the table
        lockSem(semId, TABLE_SEM);
        placePizzaOnTable(table1, type);
        printf("[Cook]  PID: %d Time: %s  ->   Podaje pizze do stolu: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n",
               getpid(), getCurrentTime(), type, oven1->pizzas, table1->pizzas);
        unlockSem(semId, TABLE_SEM);

        unlockSem(semId, EMPTY_TABLE_SEM);

    }
}
