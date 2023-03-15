//
// Created by ppjas on 5/7/2022.
//

#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#ifndef Z6_HEADER_H
#define Z6_HEADER_H

#endif //Z6_HEADER_H

#define ID 'K'

// MTYPES
#define STOP 1
#define LIST 2
#define ONE 3
#define ALL 4
#define INIT 5


#define MAX_LEN 128
#define MAX_CLIENTS 10
#define MSG_SIZE sizeof(message) - sizeof(long)


typedef struct {
    long type;
    char text[MAX_LEN];
    int q_id;
    pid_t sender_pid;
    int sender_id;
    int receiver_id;
    time_t send_time;
} message;

typedef struct {
    int client_queue;
    pid_t client_pid;
    bool used;
} client;

char *home_path() {
    char *path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}

void sendMes(int queue, message *msg) {
    if (msgsnd(queue, msg, MSG_SIZE, 0) == -1) {
        printf("error while sending message\n");
        exit(5);
    }
}

// if no msg, calling process is blocked
void receiveMes(int queue, message *msg, long type) {
    if (msgrcv(queue, msg, MSG_SIZE, type, 0) == -1) {
        printf("Error while receiving message!\n");
        exit(1);
    }
}

// calling process is not blocked
void receiveMsgNowait(int q_id, message *msg, long type) {
    if (msgrcv(q_id, msg, MSG_SIZE, type, IPC_NOWAIT) == -1) {
        printf("Error while receiving message (NOWAIT)!\n");
        if (errno == ENOMSG) {
            printf("No message!\n");
        }
        exit(1);
    }
}

void delete_queue(int queue) {
    msgctl(queue, IPC_RMID, NULL);
}
