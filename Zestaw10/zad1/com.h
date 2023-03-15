//
// Created by ppjas on 6/1/2022.
//

#ifndef LAB10_COM_H
#define LAB10_COM_H


#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define clientsMaximum 10
#define MAX_MSG_LEN 256

typedef struct {
    char *name;
    int fd;
    bool available;
    int opponent_idx;
} client;

typedef enum {
    START,
    WAIT_FOR_OPPONENT,
    WAIT_FOR_MOVE,
    OPPONENT_MOVE,
    MOVE,
    QUIT
} state;

typedef enum {
    FREE,
    O,
    X
} sign;

typedef struct {
    int move;
    sign objects[9];

} game_board;


#endif
