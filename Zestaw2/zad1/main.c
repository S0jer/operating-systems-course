#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>


long double time_sec(clock_t time) {
    return (long double) (time) / sysconf(_SC_CLK_TCK);
}


void copyLib(char *file1, char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "a");

    if (f1 == NULL || f2 == NULL) {
        perror("Can not open a file.");
        exit(1);
    }
    char *line;
    char buffer;
    int size = 0;
    int copy = 0;

    while (true) {

        fread(&buffer, sizeof(char), 1, f1);
        size++;
        if ((buffer == '\n' && copy == 1)) {
            fseek(f1, -1 * (size), 1);
            line = calloc(sizeof(char), size - 1);
            fread(line, sizeof(char), size - 1, f1);
            fread(&buffer, sizeof(char), 1, f1);
            fwrite(line, sizeof(char), strlen(line), f2);
            copy = 0;
            size = 0;
        } else if (buffer == '\n' && copy == 0) {
            size = 0;
        }
        if (!isspace(buffer)) {
            copy = 1;
        }
        if (feof(f1)) break;

    }
    if (copy == 1) {
        fseek(f1, -1 * (size), 1);
        line = calloc(sizeof(char), size - 1);
        fread(line, sizeof(char), size - 1, f1);
        fwrite(line, sizeof(char), strlen(line), f2);
    }

    fclose(f1);
    fclose(f2);
    free(line);
}


void copySys(char *file1, char *file2) {
    int f1 = open(file1, O_RDONLY);
    int f2 = open(file2, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (f1 < 0) {
        perror("Can not open a file.");
        exit(1);
    }
    char *line;
    char buffer;
    int size = 1;
    int copy = 0;

    while (true) {

        read(f1, &buffer, sizeof(char));
        size++;
        if (read(f1, &buffer, sizeof(char)) == 0) break;


        lseek(f1, -1 * (size), SEEK_CUR);
        line = calloc(sizeof(char), (size - 1));
        read(f1, line, (size - 1) * sizeof(char));
        if (!isspace(buffer)) {
            copy = 1;
        }

        if ((buffer == '\n' && copy == 1)) {
            write(f2, line, sizeof(char) * strlen(line));
            read(f1, &buffer, sizeof(char));
            copy = 0;
            size = 0;
        } else if (buffer == '\n' && copy == 0) {
            size = 0;
        }

    }

    if (copy == 1) {
        lseek(f1, -1 * (size), SEEK_CUR);
        line = calloc(sizeof(char), (size - 1));
        read(f1, line, (size - 1) * sizeof(char));
        write(f2, line, strlen(line) * sizeof(char));
    }

    close(f1);
    close(f2);
    free(line);
}


int main(int argc, char *argv[]) {
    char *file1;
    char *file2;


    if (argc == 3) {
        file1 = argv[1];
        file2 = argv[2];
        printf("Loaded files: \n");
        printf("%s %s\n ", file1, file2);
    } else if (argc == 2) {
        file1 = argv[1];
        file2 = calloc(256, sizeof(char));
        printf("Second argument: \n");
        scanf("%s", file2);
        printf("Loaded files: \n");
        printf("%s %s\n ", file1, file2);
    } else {
        file1 = calloc(256, sizeof(char));
        file2 = calloc(256, sizeof(char));
        printf("Arguments: \n");
        printf("File1: ");
        scanf("%s", file1);
        printf("File2: ");
        scanf("%s", file2);
        printf("Loaded files: \n");
        printf("%s %s\n", file1, file2);
    }

    copySys(file1, file2);
    copyLib(file1, file2);


}

