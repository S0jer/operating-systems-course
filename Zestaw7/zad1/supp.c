//
// Created by ppjas on 5/10/2022.
//


#include "shared.h"


int takePizza(table *table1) {
    int type = table1->arr[table1->takeOutIdx];
    table1->arr[table1->takeOutIdx] = -1;
    table1->takeOutIdx++;
    table1->takeOutIdx = table1->takeOutIdx % TABLE_SIZE;
    table1->pizzas--;

    return type;
}


int main() {
    int semId = getSemId();
    int shmTableId = getShmTableId();
    table *table1 = shmat(shmTableId, NULL, 0);
    int type;

    while (true) {
        lockSem(semId, EMPTY_TABLE_SEM);

        // Take out pizza
        lockSem(semId, TABLE_SEM);
        type = takePizza(table1);
        printf("[Supp] PID: %d Time: %s  ->   Wyciagam pizze: %d. Pizze na stole: %d.\n", getpid(),
               getCurrentTime(), type, table1->pizzas);
        unlockSem(semId, TABLE_SEM);

        unlockSem(semId, FULL_TABLE_SEM);
        sleep(DELIVERY_TIME);

        printf("[Supp] PID: %d Time: %s  ->   Dostarczam pizze: %d.\n", getpid(), getCurrentTime(), type);
        sleep(RETURN_TIME);
    }
}