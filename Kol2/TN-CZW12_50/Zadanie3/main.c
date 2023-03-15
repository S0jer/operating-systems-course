#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <strings.h>
#include <unistd.h>


/*
 * Funkcja 'uname' przyjmuje socket domeny unix ('s') i jego
 * docelowa nazwe ('name'). Funkcja 'uname' powinna:
 *   - nadac socketowi 's' nazwe 'name',
 *   - zwrocic 0 w przypadku powodzenia i -1 w przypadku bledu.
 */
int uname(int s, char *name){
    // Uzupelnij cialo funkcji uname zgodnie z
    // komentarzem powyzej.
}

/*
 * Funkcja 'uconnect' przyjmuje socket domeny unix ('s') i jego
 * docelowy adres ('name'). Funkcja 'uconnect' powinna:
 *   - polaczyc socket 's' z adresem 'name',
 *   - zwrocic 0 w przypadku powodzenia i -1 w przypadku bledu.
 */
int uconnect(int s, char *name){
    // Uzupelnij cialo funkcji uconnect zgodnie z
    // komentarzem powyzej
}

int main(int argc, char *argv[]) {
    if (argc < 2) return -1;
    if(fork() > 0) {
        char bufout[4];
        int s = socket(AF_UNIX, SOCK_STREAM, 0);
        if(uname(s, argv[1]) || listen(s, 1)) return -1;
        int fd = accept(s, NULL, 0);
        read(fd, bufout, 4);
        printf("out: %4s\n", bufout);
    } else {
        char bufin[4];
        int i;
        for(i=0;i<4;i++) bufin[i]=(rand()%26+'a');
        int c = socket(AF_UNIX, SOCK_STREAM, 0);
        sleep(1);
        if(uconnect(c, argv[1])) return -1;
        write(c, bufin, 4);
        printf("in:  %4s\n", bufin);
    }

    return 0;
}
