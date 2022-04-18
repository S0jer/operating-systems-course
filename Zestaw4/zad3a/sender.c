//
// Created by ppjas on 4/4/2022.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int caught = 0;
int toSend;
char *mode;


void handler1(int sig, siginfo_t *info, void *uct) {
    caught++;
    if (strcmp(mode, "queue") == 0) {
        printf("Number of signal sent back from catcher: %d \n", info->si_value.sival_int);
    }
}

void handler2(int signum) {
    printf("\n ------- ALL SIGNALS SENT BACK ------- \n\n");
    printf("\n Sender caught %d / %d SIGUSR signals. \n", caught, toSend);
    exit(0);
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments :) \n");
        return -1;
    }

    int catcherPID = atoi(argv[1]);
    toSend = atoi(argv[2]);
    mode = argv[3];

    printf("\n\nSender PID: %d \n", getpid());
    printf("Catcher PID: %d \n", catcherPID);
    printf("Number of signals to send: %d \n", toSend);
    printf("Mode: %s \n", mode);

    struct sigaction actionSIGUSR1;
    sigemptyset(&actionSIGUSR1.sa_mask);
    actionSIGUSR1.sa_flags = SA_SIGINFO;
    actionSIGUSR1.sa_sigaction = handler1;
    if (strcmp(mode, "sigrt") != 0) {
        sigaction(SIGUSR1, &actionSIGUSR1, NULL);
    } else {
        sigaction(SIGRTMIN + 1, &actionSIGUSR1, NULL);
    }

    struct sigaction actionSIGUSR2;
    sigemptyset(&actionSIGUSR2.sa_mask);
    actionSIGUSR2.sa_handler = handler2;
    if (strcmp(mode, "sigrt") != 0) {
        sigaction(SIGUSR2, &actionSIGUSR2, NULL);
    } else {
        sigaction(SIGRTMIN + 2, &actionSIGUSR2, NULL);
    }

    for (int i = 0; i < toSend; i++) {
        printf("\n Sending SIGUSR1 number: %d \n", i + 1);
        if (strcmp(mode, "kill") == 0) {
            kill(catcherPID, SIGUSR1);
        } else if (strcmp(mode, "queue") == 0) {
            union sigval value;
            sigqueue(catcherPID, SIGUSR1, value);
        } else if (strcmp(mode, "sigrt") == 0) {
            kill(catcherPID, SIGRTMIN + 1);
        } else {
            printf("Wrong mode\n");
            return -1;
        }
    }
    printf("SIGUSR1 send, sending SIGUSR2");
    if (strcmp(mode, "kill") == 0) {
        kill(catcherPID, SIGUSR2);
    } else if (strcmp(mode, "queue") == 0) {
        union sigval value;
        sigqueue(catcherPID, SIGUSR2, value);
    } else if (strcmp(mode, "sigrt") == 0) {
        kill(catcherPID, SIGRTMIN + 2);
    } else {
        printf("Wrong mode\n");
        return -1;
    }
    while (1) {}
}


