#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE "./squareFIFO"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    if (mkfifo(PIPE, 0777) == -1) {
        printf("Couldn't create PIPE \n");
        return -1;
    }
    int fd = open(PIPE, O_WRONLY);
    if (fd == -1) {
        printf("Couldn't open PIPE\n");
        return -1;
    }
    write(fd, argv[1], sizeof(argv[1]));
    close(fd);

    // utworz potok nazwany pod sciezka reprezentowana przez PIPE
    // zakladajac ze parametrem wywolania programu jest liczba calkowita
    // zapisz te wartosc jako int do potoku i posprzataj

    return 0;
}
