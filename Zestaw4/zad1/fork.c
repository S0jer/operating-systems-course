//
// Created by ppjas on 4/1/2022.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


void handler(int signum) {
    printf("Signal %d. | PID: %d | PPID: %d \n", signum, getpid(), getppid());
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong number of arguments :) \n");
        return -1;
    } else if (strcmp(argv[1], "ignore") == 0) {
        printf("Test: ignore \n");
        signal(SIGUSR1, SIG_IGN);
        printf("Raise a signal \n");
        raise(SIGUSR1); // kill(getpid(), SIGUSR1);
        pid_t pid = fork();
        if (pid == 0) {
            printf("Raise a signal [child] \n");
            raise(SIGUSR1);
        } else {
            wait(NULL);
        }
    } else if (strcmp(argv[1], "handler") == 0) {
        printf("Test: handler \n");
        signal(SIGUSR1, handler);
        printf("Raise a signal \n");
        raise(SIGUSR1); // kill(getpid(), SIGUSR1);
        pid_t pid = fork();
        if (pid == 0) {
            printf("Raise a signal [child] \n");
            raise(SIGUSR1);
        } else {
            wait(NULL);
        }
    } else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) {
        if (strcmp(argv[1], "mask") == 0) {
            printf("Test: mask \n");
        } else { printf("Test: pending \n"); }

        sigset_t nMask;
        sigset_t oMask;
        sigemptyset(&nMask);
        sigaddset(&nMask, SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &nMask, &oMask) < 0) {
            perror("Signal blocking failed \n");
        }

        printf("Raise a signal \n");
        raise(SIGUSR1);

        sigismember(&nMask, SIGUSR1) ? printf("Signal pending \n") : printf("Signal not pending \n");

        pid_t pid = fork();
        if (pid == 0) {
            if (strcmp(argv[1], "mask") == 0) {
                printf("Rise a signal [child] \n");
                raise(SIGUSR1);
            } else
                printf("Child process \n");

            sigpending(&nMask);
            sigismember(&nMask, SIGUSR1) ? printf("Signal pending [child] \n") : printf(
                    "Signal not pending [child] \n");
        } else {
            wait(NULL);
        }
    }

    return 0;
}
