//
// Created by ppjas on 4/21/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 256

void Output(FILE *file) {
    char line[MAX_SIZE];
    while (fgets(line, MAX_SIZE, file) != NULL) {
        printf("%s\n", line);
    }
}

void sendEmail(char *address, char *object, char *content) {
    FILE *file;
    char com[MAX_SIZE];
    snprintf(com, sizeof(com), "echo %s | mail -s %s %s", content, object, address);
    printf("%s\n", com);
    file = popen(com, "r");
    if (file == NULL) {
        printf("popen() error!");
        exit(1);
    }
    printf("\n Email sent \n");
    printf("To:      %s\n o:  %s\n c: %s\n", address, object, content);
}

void OrderedBy(char *mode) {
    FILE *file;
    char *com;
    if (strcmp(mode, "date") == 0) {
        com = "echo | mail -f | tail +2 | head -n -1 | tac";
    } else if (strcmp(mode, "sender") == 0) {
        com = "echo | mail -f | tail +2 | head -n -1 | sort -k 2";
    } else {
        printf("Wrong mode! \n");
        exit(1);
    }
    file = popen(com, "r");
    if (file == NULL) {
        printf("popen() error!");
        exit(1);
    }
    printf("\nSORTED EMAILS: ");
    Output(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 4) {
        printf("Wrong number of arguments :)");
        exit(1);
    }
    if (argc == 2) {
        printf("Print emails ordered by [%s] \n", argv[1]);
        OrderedBy(argv[1]);
    } else {
        printf("Send email \n");
        char *address = argv[1];
        char *subject = argv[2];
        char *content = argv[3];
        sendEmail(address, subject, content);
    }
}
