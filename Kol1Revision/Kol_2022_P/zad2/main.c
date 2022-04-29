#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv) {
    if (argc == 2) {
        char *filename1 = argv[1];
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0) {
            char buf[32];

            //  zamknij odpowiedni deskryptor, odczytaj napis (komendę) z potoku i wykonaj tę komendę na filename1
            close(fd[1]);
            if (read(fd[0], &buf, 32 * sizeof(char)) <= 0){
                return 3;
            }
            if(execl(buf, buf, filename1) == -1){
                return 3;
            }
            //  w przypadku błędu zwróć 3

            //  koniec
        } else {
            close(fd[0]);
            char s[] = "sort";
            write(fd[1], &s, sizeof(s));
            // Wyślij napis "sort" poprzez potok
        }
    } else if (argc == 3) {
        char *filename1 = argv[1];
        char *filename2 = argv[2];
        int fd[2];
        int file2 = open(filename2, O_WRONLY | O_CREAT, 0744);
        //  otwórz plik filename2 z prawami dostępu rwxr--r--,
        //  jeśli plik istnieje otwórz go i usuń jego zawartość

        //  koniec
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0) {
            close(fd[1]);
            dup2(fd[0] ,STDOUT_FILENO);

//            execl(qsort, qsort, filename1);
            execl("/usr/bin/sort", "usr/bin/sort", "-n","-k", "1", "-u", filename1, (char*)NULL);
            close(file2);

            //  zamknij deskryptor do zapisu,
            //  przekieruj deskryptor standardowego wyjścia na deskryptor pliku filename2 i zamknij plik,
            //  wykonaj program sort na filename1
            //  w przypadku błędu zwróć 3.

            //  koniec
        } else {
            close(fd[0]);
        }
    } else
        printf("Wrong number of args! \n");

    return 0;
}
