//
// Created by ppjas on 4/3/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


int main(int argc, char *argv[]) {
    printf("Child for exec \n");

    if (strcmp(argv[1], "ignore") == 0) {
        printf("Raise a signal [child] \n");
        raise(SIGUSR1);
    } else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) {
        if (strcmp(argv[1], "mask") == 0) {
            printf("Raise a signal [child] \n");
            raise(SIGUSR1);
        }

        sigset_t new_mask;
        sigpending(&new_mask);
        sigismember(&new_mask, SIGUSR1) ? printf("Signal pending [child]\n") : printf(
                "Signal not pending [child]\n");
    }

    return 0;
}

