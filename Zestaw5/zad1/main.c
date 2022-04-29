//
// Created by ppjas on 4/21/2022.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#define MAX_ARGS 20
#define MAX_LINES 10
#define READ 0
#define WRITE 1


int getLineNumber(char *line, int i) {
    if (i == 0) return line[8] - '0';
    else return line[9] - '0';
}

int *getLines(char *line) {
    char **lines = (char **) calloc(MAX_ARGS, sizeof(char *));
    char *arg = strtok(line, "|");
    int ctr = 0;
    lines[ctr++] = arg;

    while ((arg = strtok(NULL, "|")) != NULL) {
        lines[ctr++] = arg;
    }
    static int linesNum[MAX_ARGS];
    int i = 0;
    while (lines[i] != NULL) {
        linesNum[i] = getLineNumber(lines[i], i);
        i++;
    }
    linesNum[i] = -1;

    return linesNum;
}

char **getProgramArgs(char *com, char *path) {
    char **args = (char **) calloc(256, sizeof(char *));

    char *arg = strtok(com, " ");
    strcpy(path, arg);
    int ctr = 0;
    args[ctr++] = arg;
    while ((arg = strtok(NULL, " ")) != NULL) {
        args[ctr++] = arg;
    }
    args[ctr] = NULL;

    return args;
}

char **p(char *line) {
    char **com = (char **) calloc(MAX_ARGS, sizeof(char *));
    char *seq = strtok(line, "=");
    int ctr = 0;
    while ((seq = strtok(NULL, "|")) != NULL) {
        com[ctr++] = seq;
    }
    return com;
}

void parse(FILE *file) {
    char **lines = (char **) calloc(MAX_LINES, sizeof(char *));
    char **com; char **args; char *currentLine;
    char *line = (char *) calloc(256, sizeof(char));
    int *linesNum; int lineCounter = 0; int linesNumber; int i;

    while (fgets(line, 256 * sizeof(char), file)) {
        printf("\n LINE: %s", line);

        if (strstr(line, "=")) {
            char *lineCopy = (char *) calloc(256, sizeof(char));
            strcpy(lineCopy, line);
            lines[lineCounter++] = lineCopy;
        } else {
            linesNum = getLines(line);
            i = 0;
            while (linesNum[i] != -1) {
                printf("L %d \n", linesNum[i]);
                i++;
            }
            int pipeIn[2];
            int pipeOut[2];
            if (pipe(pipeOut) != 0) {
                printf("Error while creating a pipe!\n");
                exit(1);
            }

            linesNumber = 0;
            while (linesNum[linesNumber] != -1) linesNumber++;
            for (int j = 0; j < linesNumber; j++) {
                currentLine = lines[linesNum[j]];
                printf("\nLine %d:  %s \n", j, currentLine);
                com = p(lines[linesNum[j]]);
                pid_t pid;
                int m = 0;
                while (com[m] != NULL) {
                    printf("com%d:  %s\n", m + 1, com[m]);
                    m++;
                }
                for (int k = 0; k < m; k++) {
                    pid = fork();
                    if (pid == 0) {
                        if (k == 0 && j == 0) {
                            close(pipeOut[READ]);
                            dup2(pipeOut[WRITE], STDOUT_FILENO);
                        } else if (k == m - 1 && j == linesNumber - 1) {
                            close(pipeOut[READ]);
                            close(pipeOut[WRITE]);
                            close(pipeIn[WRITE]);
                            dup2(pipeIn[READ], STDIN_FILENO);
                        } else {
                            close(pipeIn[WRITE]);
                            close(pipeOut[READ]);
                            dup2(pipeIn[READ], STDIN_FILENO);
                            dup2(pipeOut[WRITE], STDOUT_FILENO);
                        }
                        char path[256];
                        args = getProgramArgs(com[k], path);
                        printf(" EXEC \npath: %s\n", path);
                        m = 0;
                        while (args[m] != NULL) {
                            printf("arg%d: %s\n", m + 1, args[m]);
                            m++;
                        }
                        if (execvp(path, args) == -1) {
                            printf("ERROR in exec\n");
                            exit(1);
                        }
                    } else {
                        close(pipeIn[WRITE]);
                        pipeIn[READ] = pipeOut[READ];
                        pipeIn[WRITE] = pipeOut[WRITE];

                        if (pipe(pipeOut) != 0) {
                            printf("Error while moving pipe!\n");
                            exit(1);
                        }
                    }
                }
            }
        }
        int status = 0;
        pid_t wpid;
        while ((wpid = wait(&status)) != -1);
        printf("\nALL CHILDREN TERMINATED\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments ! :) \n");
        exit(1);
    }
    char *path = argv[1];
    FILE *com = fopen(path, "r");
    if (com == NULL) {
        printf("Can not open file :) \n");
        exit(1);
    }
    parse(com);
    fclose(com);
}
