//
// Created by ppjas on 4/3/2022.
//

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/times.h>


volatile sig_atomic_t recursive_sig1 = 0;
volatile sig_atomic_t recursive_sig2 = 0;

void nodeffer1_handler(int sig);

void nodeffer2_handler(int sig);

void siginfo_handler(int signum, siginfo_t *info, void *ctx);

void RESETHAND_handler(int sig);


void testSIGINFO(){
    printf("\n ####### TEST SA_SIGINFO ####### \n");
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = siginfo_handler;
    sigemptyset(&action.sa_mask);

    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    raise(SIGUSR1);
    raise(SIGUSR2);

}


void testRESETHAND() {
    printf("\n ####### TEST SA_RESETHAND ####### \n");
    struct sigaction action;
    action.sa_handler = RESETHAND_handler;
    action.sa_flags = SA_RESETHAND;
    sigemptyset(&action.sa_mask);


    sigaction(SIGUSR1, &action, NULL);

    raise(SIGUSR1);
    raise(SIGUSR1);

}


void testNODEFER(){
    printf("\n ####### TEST SA_NODEFER ####### \n");
    {
        struct sigaction action;
        action.sa_handler = nodeffer1_handler;
        action.sa_flags = 0;
        sigemptyset(&action.sa_mask);
        sigaction(SIGUSR1, &action, NULL);

    }
    {
        struct sigaction action;
        action.sa_handler = nodeffer2_handler;
        action.sa_flags = SA_NODEFER;
        sigemptyset(&action.sa_mask);

        sigaction(SIGUSR2, &action, NULL);
    }

    raise(SIGUSR1);
    raise(SIGUSR2);

}



int main() {
    testSIGINFO();
    testNODEFER();
    testRESETHAND();

    return 0;
}



void RESETHAND_handler(int sig) {
    printf("\n SIGUSR1 received \n");
}


void siginfo_handler(int signum, siginfo_t *info, void *ctx) {
    printf("\n\nReceived signal %d | PID: %d | PPID: %d \n", info->si_signo, info->si_pid, getppid());
    printf("Status: %d\n", info->si_status);
    printf("Sender UID: %d\n", info->si_uid);
    printf("User time: %ld\n", info->si_utime);
    printf("Signal code: %d\n", info->si_code);
    printf("Errno value: %d\n", info->si_errno);
}


void nodeffer1_handler(int sig) {
    if (recursive_sig1 == 0) {
        recursive_sig1 = 1;
        raise(SIGUSR1);
    }
    else if (recursive_sig1 == 1) {
        printf("\n SIGUSR1 recursive call \n");
    }
    recursive_sig1 = 2;
}


void nodeffer2_handler(int sig) {
    if (recursive_sig2 == 0) {
        recursive_sig2 = 1;
        raise(SIGUSR2);
    }
    else if (recursive_sig2 == 1) {
        printf("\n SIGUSR2 recursive call \n");
    }
    recursive_sig2 = 2;
}

