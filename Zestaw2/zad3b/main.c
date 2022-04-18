//
// Created by ppjas on 3/23/2022.
//

#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>


int dirCounter;
int regCounter;
int bloCounter;
int chrCounter;
int socCounter;
int symCounter;


void printType(mode_t iMode) {
    if (S_ISREG(iMode)) {
        printf("File Type: \t\tregular file\n");
        return;
    }
    if (S_ISDIR(iMode)) {
        printf("File Type: \t\tdirectory\n");
        return;
    }
    if (S_ISBLK(iMode)) {
        printf("File Type: \t\tblock device\n");
        return;
    }
    if (S_ISCHR(iMode)) {
        printf("File Type: \t\tcharacter device\n");
        return;
    }
    if (S_ISSOCK(iMode)) {
        printf("File Type: \t\tsocket\n");
        return;
    }
    if (S_ISLNK(iMode)) {
        printf("File Type: \t\tsymbolic link\n");
        return;
    }

}

void printPermission(mode_t iMode) {
    printf("File Permissions: \t");
    printf((S_ISDIR(iMode)) ? "d" : "-");
    printf((iMode & S_IRUSR) ? "r" : "-");
    printf((iMode & S_IWUSR) ? "w" : "-");
    printf((iMode & S_IXUSR) ? "x" : "-");
    printf((iMode & S_IRGRP) ? "r" : "-");
    printf((iMode & S_IWGRP) ? "w" : "-");
    printf((iMode & S_IXGRP) ? "x" : "-");
    printf((iMode & S_IROTH) ? "r" : "-");
    printf((iMode & S_IWOTH) ? "w" : "-");
    printf((iMode & S_IXOTH) ? "x" : "-");
    printf("\n\n");

}


static int printFiles(char *path, struct stat *fileStat, int tflag, struct FTW *ftwbuf) {


    struct stat attrib;
    int fileCounter[10];


    stat(path, fileStat);
    stat(path, &attrib);

    if (tflag == FTW_NS) {
        printf("----------");
    } else {

        char *lastMod = calloc(sizeof(char), 256);
        char *lastOpen = calloc(sizeof(char), 256);
        strftime(lastMod, 20, "%d-%m-%y", localtime(&(attrib.st_mtime)));
        strftime(lastOpen, 20, "%d-%m-%y", localtime(&(attrib.st_atime)));

        printf("Information for %s\n", path);
        printf("---------------------------\n");
        printf("File Size: \t\t%ld bytes\n", fileStat->st_size);
        printType(fileStat->st_mode);
        printf("Number of Links: \t%ld\n", fileStat->st_nlink);
        printf("File inode: \t\t%ld\n", fileStat->st_ino);
        printf("The file was last opened at %s\n", lastOpen);
        printf("The file was last modified at %s\n", lastMod);
        printPermission(fileStat->st_mode);


        if (S_ISREG(fileStat->st_mode)) {
            regCounter += 1;
        }
        if (S_ISDIR(fileStat->st_mode)) {
            dirCounter += 1;
        }
        if (S_ISBLK(fileStat->st_mode)) {
            bloCounter += 1;
        }
        if (S_ISCHR(fileStat->st_mode)) {
            chrCounter += 1;
        }
        if (S_ISSOCK(fileStat->st_mode)) {
            socCounter += 1;
        }
        if (S_ISLNK(fileStat->st_mode)) {
            symCounter += 1;
        }


        free(lastMod);
        free(lastOpen);

    }

    return 0;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments :) \n");
        exit(1);
    }
    int flags = FTW_PHYS;
    void *func = printFiles;

    char *dirName = argv[1];
    nftw(dirName, func, 5, flags);

    printf("\n \t Number of files by type: \n");
    printf("Regular file %d\n", regCounter);
    printf("Directory %d\n", dirCounter);
    printf("Block device %d\n", bloCounter);
    printf("Character device %d\n", chrCounter);
    printf("Socket %d\n", socCounter);
    printf("Symbolic link %d\n", symCounter);

    return 0;

}