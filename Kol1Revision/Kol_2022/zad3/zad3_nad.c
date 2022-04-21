#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int i=0;
/*
program przy kazdym uzyciu klawiszy ctrl-c ma wyslac przez potok
nazwany 'potok1' zawarto¶æ zmiennej 'i' */

//
//

int main (int lpar, char *tab[]){
//
//
  while(1) {
    printf("%d ",i++); fflush(stdout);
    sleep(1);
  }
  return 0;
}
