#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>


/* 1) wywolaj funkcje 'licz' dla parametrow od 1 do 20. Kazde wywolanie tej
   funkcji ma byc w innym procesie potomnym
   2) proces macierzysty ma wyswietlic pid wszystkich procesow potomnych i
   czekac na zakonczenie ich wszystkich*/



void licz(int x) {
    printf("dla x=%d wynik x2 to %d w procesie o PID: %d\n", x, x * x, getpid());
    fflush(stdout);
    exit(0);
}


int main(int lpar, char *tab[]) {
    pid_t pid;
    int i = 1;
    int status = 0;

    for (i = 1; i <= 20; i++) {
        pid = fork();
        if (pid == 0) {
            licz(i);
            wait(&status);
        }
    }

    for (int j = 1; j <= 20; j++) {
        wait(NULL);
    }

    return 0;
}
