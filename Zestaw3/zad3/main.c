//
// Created by ppjas on 3/23/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>


void searchFor(char *dirName, char *strToFind, int depth) {

    if (depth == 0) exit(0);

    FILE *f;
    DIR *dir;
    struct dirent *dp;
    char *path;
    char *fileExt;
    bool found;

    if ((dir = opendir(dirName)) == NULL) {
        perror("Can not open directory \n");
        exit(1);
    }

    while ((dp = readdir(dir))) {
        if ((strcmp(dp->d_name, "..") == 0) || (strcmp(dp->d_name, ".") == 0)) continue;
        path = calloc(sizeof(char), strlen(dirName) + strlen(dp->d_name));
        sprintf(path, "%s/%s", dirName, dp->d_name);
        fileExt = strrchr(dp->d_name, '.');


        if (dp->d_type == DT_DIR && fork() == 0) {
            searchFor(path, strToFind, depth - 1);
            exit(0);
        } else if (fileExt != NULL && strcmp(fileExt, ".txt") == 0) {
            f = fopen(path, "r");
            if (f == NULL) {
                perror("Can not open file");
                exit(1);
            }

            char *l = calloc(sizeof(char), 256);
            while (fgets(l, 256 * sizeof(char), f)) {
                if (strstr(l, strToFind)) found = true;
                else found = false;
            }
            if (found) {
                printf("\npath: %s\n", path);
                printf("PID:  %d \nPPID: %d\n\n", getpid(), getppid());
            }

            free(l);

        }

        free(path);
        wait(NULL);

    }

    closedir(dir);
}


int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Wrong number of arguments :) \n");
        exit(1);
    }

    char *dirName = argv[1];
    char *strToFind = argv[2];
    int depth = atoi(argv[3]);

    searchFor(dirName, strToFind, depth);

    return 0;
}

