//
// Created by ppjas on 4/4/2022.
//


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

int caught = 0;
char *mode;


void handler1(int signum) {
    caught++;
}

void handler2(int sig, siginfo_t *info, void *uct) {
    printf("\n ------- ALL SIGNALS SENT ------- \n");

    pid_t senderPID = info->si_pid;

    for (int i = 0; i < caught; i++) {
        printf("\n Sending back SIGUSR1 number: %d \n", i + 1);
        if (strcmp(mode, "kill") == 0) {
            kill(senderPID, SIGUSR1);
        } else if (strcmp(mode, "queue") == 0) {
            union sigval valuee;
            valuee.sival_int = i;
            sigqueue(senderPID, SIGUSR1, valuee);
        } else if (strcmp(mode, "sigrt") == 0) {
            kill(senderPID, SIGRTMIN + 1);
        } else {
            printf("Wrong mode\n");
        }
    }
    if (strcmp(mode, "kill") == 0) {
        kill(senderPID, SIGUSR2);
    } else if (strcmp(mode, "queue") == 0) {
        union sigval valuee;
        sigqueue(senderPID, SIGUSR2, valuee);
    } else if (strcmp(mode, "sigrt") == 0) {
        kill(senderPID, SIGRTMIN + 2);
    } else {
        printf("Wrong mode\n");
    }
    exit(0);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments :) \n");
        return -1;
    }
    printf("\n Catcher PID: %d \n", getpid());
    mode = argv[1];

    struct sigaction actionSIGUSR1;
    sigemptyset(&actionSIGUSR1.sa_mask);
    actionSIGUSR1.sa_handler = handler1;
    if (strcmp(mode, "sigrt") != 0) {
        sigaction(SIGUSR1, &actionSIGUSR1, NULL);
    } else {
        sigaction(SIGRTMIN + 1, &actionSIGUSR1, NULL);
    }

    struct sigaction actionSIGUSR2;
    sigemptyset(&actionSIGUSR2.sa_mask);
    actionSIGUSR2.sa_flags = SA_SIGINFO;
    actionSIGUSR2.sa_sigaction = handler2;
    if (strcmp(mode, "sigrt") != 0) {
        sigaction(SIGUSR2, &actionSIGUSR2, NULL);
    } else {
        sigaction(SIGRTMIN + 2, &actionSIGUSR2, NULL);
    }

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGUSR2);
    if (strcmp(mode, "sigrt") == 0) {
        sigdelset(&mask, SIGRTMIN + 1);
        sigdelset(&mask, SIGRTMIN + 2);
    }
    sigprocmask(SIG_SETMASK, &mask, NULL);

    while (1) {
        sigsuspend(&mask);
    }
}

