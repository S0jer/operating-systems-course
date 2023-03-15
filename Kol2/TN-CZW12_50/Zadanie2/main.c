#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

/*
 * Funkcja 'create_mutex' powinna zainicjalizować mutex w taki sposób, by w sytuacji,
 * gdy wątek próbuje zablokować mutex, który już jest zablokowany, liczba blokad się zwiększała
 * i nie następowało zakleszczenie. Odblokowanie ma następować po zniwelowaniu poziomu blokad
 * do zera.
 */
void create_mutex(pthread_mutex_t *mutex) {

}


int main(void) {
    pthread_mutex_t mutex;
    create_mutex(&mutex);
    int ret;
    int i = 0;

    while (i < 12) {
        if (i < 5 || i > 10) {
            ret = pthread_mutex_lock(&mutex);
            printf("lock - ");
        } else {
            ret = pthread_mutex_unlock(&mutex);
            printf("unlock - ");
        }

        switch (ret) {
            case 0:
                printf("locked\n");
                break;
            case 1:
                printf("unlocked\n");
                break;
            case EDEADLK:
                printf("To nie jest prawidłowy typ mutexu\n");
                exit(-1);
                break;
        }
        i++;
    }
    pthread_mutex_destroy(&mutex);
}

