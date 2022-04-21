#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int lpar, char *tab[]) {
    int w1;
    char buf[1024];
// 1) utworzyc potok nazwany 'potok1'
// 2) wyswietlac liczby otrzymywane z potoku

    while (1) {
//
//
        printf("otrzymano: %s\n", buf);
        fflush(stdout);
    }

    return 0;
}
