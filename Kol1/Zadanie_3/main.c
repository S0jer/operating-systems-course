

/* pipe_ls_wc.c
   Program wykonuje instukcje potoku "ls | wc -l" 
   Uzupelnij wlasciwe instrukcje w miejscach *****
*/

#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int
main(int argc, char *argv[]) {
    int pfd[2];

    if (pipe(pfd) == -1) {
        fprintf(stderr, "pipe");
        exit(-1);
    }
    switch (fork()) {
        case -1:
            fprintf(stderr, "fork");
            exit(-1);

        case 0:
            if (close(pfd[0]) == -1) {
                fprintf(stderr, "close 1");
                exit(-1);
            }


            if (pfd[1] != STDOUT_FILENO) {
                if (dup2(pfd[1], STDOUT_FILENO) == -1) {
                    fprintf(stderr, "dup2 1");
                    exit(-1);
                }

                if (close(pfd[2]) == -1) {
                    fprintf(stderr, "close 2");
                    exit(-1);
                }


            }

            /***** Uruchom proces ls *****/
//            execl("/user/bin/ls", "/user/bin/ls", NULL);
            char cmdls[] = "ls";
            system(cmdls);

            exit(-1);

        default:
            break;
    }

    switch (fork()) {
        case -1:
            fprintf(stderr, "fork");
            exit(-1);
        case 0:
            if (close(pfd[1]) == -1) {
                fprintf(stderr, "close 3");
                exit(-1);
            }


            /***** uzyj dup2 z wlasciwymi argumentami *****/
            if (pfd[2] != STDOUT_FILENO) {
                if (dup2(pfd[2], STDOUT_FILENO) == -1) {
                    fprintf(stderr, "dup2 1");
                    exit(-1);
                }
            }


            if (close(pfd[0]) == -1) {
                {
                    fprintf(stderr, "close 4");
                    exit(-1);
                }

            }
            /***** uruchom proces wc -l *****/
//            execl("/user/bin/wc", "/user/bin/wc", "-l", NULL);
            char cmd[] = "wc -l";
            system(cmd);

            exit(-1);

        default:
            break;
    }


    if (close(pfd[0]) == -1) {
        fprintf(stderr, "close 5");
        exit(-1);
    }


    if (close(pfd[1]) == -1) {
        fprintf(stderr, "close 6");
        exit(-1);
    }

    if (wait(NULL) == -1) {
        fprintf(stderr, "wait 1");
        exit(-1);
    }

    if (wait(NULL) == -1) {
        fprintf(stderr, "wait 2");
        exit(-1);
    }

    exit(EXIT_SUCCESS);
}
