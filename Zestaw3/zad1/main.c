//
// Created by ppjas on 3/22/2022.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong numbers of arguments");
        return -1;
    } else {
        printf("current: %d  parent: %d\n\n", getpid(), getppid());

        int n = atoi(argv[1]);
        pid_t pid;

        for (int i = 0; i < n; i++) {
            pid = fork();
            if (pid == 0) {
                printf("Process: %d  Child PID: %d  Parent PID: %d\n", i + 1, getpid(), getppid());
                exit(0);
            }
        }
        for (int i = 0; i < n; i++) wait(NULL);
    }

    return 0;

}



