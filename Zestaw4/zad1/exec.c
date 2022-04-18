//
// Created by ppjas on 4/3/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong number of arguments :) \n");
        return -1;
    } else if (strcmp(argv[1], "ignore") == 0) {
        printf("Test: ignore \n");
        signal(SIGUSR1, SIG_IGN);
        printf("Raise a signal \n");
        raise(SIGUSR1); // kill(getpid(), SIGUSR1);
        printf("Using execl() \n");
        execl("./childForExec", "./childForExec", argv[1], NULL);
        printf(" ! excel() error ! \n");
    } else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) {
        if (strcmp(argv[1], "pending") == 0) {
            printf("Test: pending \n");
        } else { printf("Test: mask \n"); }

        sigset_t nMask;
        sigset_t oMask;
        sigemptyset(&nMask);
        sigaddset(&nMask, SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &nMask, &oMask) < 0) {
            perror("Signal blocking failed");
        }
        printf("Raise a signal \n");
        raise(SIGUSR1);

        sigismember(&nMask, SIGUSR1) ? printf("Signal pending \n") : printf("Signal not pending \n");
        printf("Using execl() \n");
        execl("./childForExec", "./childForExec", argv[1], NULL);
        printf(" ! excel() error ! \n");
    }

    return 0;
}