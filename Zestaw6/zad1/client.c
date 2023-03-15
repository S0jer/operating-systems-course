//
// Created by ppjas on 5/7/2022.
//

#include <signal.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "com.h"

int server_queue;
int client_queue;
int client_id;

void handle_input();

void init() {
    message msg = {.q_id = client_queue, .type = INIT, .sender_pid = getpid()};
    sendMes(server_queue, &msg);

    message server_response;
    receiveMes(client_queue, &server_response, INIT);
    client_id = server_response.sender_id;
}

void delete_client_queue() {
    delete_queue(client_queue);
}

// check if there is any input available
bool check_input() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return (FD_ISSET(0, &fds));
}

void send_LIST() {
    printf("[CLIENT] Sending LIST to server...\n");
    message msg = {.type = LIST, .sender_id = client_id};
    sendMes(server_queue, &msg);
}

void send_STOP() {
    printf("[CLIENT] Sending STOP to server...\n");
    message msg = {.type = STOP, .sender_id = client_id};
    sendMes(server_queue, &msg);
}

void send_ALL(char *text) {
    printf("[CLIENT] Sending 2ALL to server...\n");
    message msg = {
            .type = ALL,
            .sender_id = client_id,
    };
    strcpy(msg.text, text);
    sendMes(server_queue, &msg);
}

void send_ONE(int receiver_id, char *text) {
    printf("[CLIENT] Sending 2ONE to server...\n");
    message msg = {
            .type = ONE,
            .sender_id = client_id,
            .receiver_id = receiver_id
    };
    strcpy(msg.text, text);
    sendMes(server_queue, &msg);
}

void handle_STOP() {
    printf("[CLIENT] STOP received...\n");

    send_STOP();
    delete_client_queue();
    exit(1);
    return 1;
}

void handle_message(message *msg) {
    time_t time = msg->send_time;
    struct tm *local_time = localtime(&time);

    if (!local_time) {
        printf("local time error\n");
        exit(15);
    }
    printf("Sender id: %d\nMessage type: %ld\nMessage body:\n%s\nTime of sending %d-%02d-%02d %02d:%02d:%02d\n\n",
           msg->sender_id,
           msg->type,
           msg->text,
           local_time->tm_year + 1900,
           local_time->tm_mon + 1,
           local_time->tm_mday,
           local_time->tm_hour,
           local_time->tm_min,
           local_time->tm_sec
    );
}

void handle_LIST(message *msg) {
    printf("\nLIST OF CLIENTS:\n");
    for (int i = 0; i < strlen(msg->text); ++i) {
        if (msg->text[i] != '\n') {
            printf("client id:%c\n", msg->text[i]);
        }
    }
}

void handle_sigint() {
    printf("[CLIENT] I received SIGINT signal..\n");
    send_STOP();
    exit(0);
}

void run() {
    struct msqid_ds queue_stat;

    pid_t pid = fork();
    if (pid == -1) {
        printf("error with creating child process\n");
        exit(9);
    }

    if (pid == 0) {
        // Child process
        while (true) {
            if (msgctl(client_queue, IPC_STAT, &queue_stat) == -1) {
                printf("get info about client queue error\n");
                if (errno == EIDRM) {
                    printf(" - queue was deleted :(\n");
                }
                exit(10);
            }
            if (queue_stat.msg_qnum) {
                message msg;
                receiveMes(client_queue, &msg, 0);
                switch (msg.type) {
                    case STOP:
                        handle_STOP();
                        exit(0);
                    case ALL:
                        handle_message(&msg);
                        break;
                    case ONE:
                        handle_message(&msg);
                        break;
                    case LIST:
                        handle_LIST(&msg);
                        break;
                    default:
                        printf("[CLIENT] I dont recognize that message type %ld\n", msg.type);
                        exit(14);
                }
            }
        }
    } else {
        // Parent process
        while (true) {
            if (check_input()) {
                handle_input();
            }
        }
    }
}

int get_receiver_id(char *rest) {
    char *receiver_id_str;
    char *rest_ = "";
    if ((receiver_id_str = strtok_r(rest, " \0", &rest_)) == NULL) {
        printf("get receiver id error\n");
        return -1;
    }

    int receiver_id = (int) strtol(receiver_id_str, NULL, 10);
    if (receiver_id < 0 || errno) {
        printf("get receiver id error\n");
        return -1;
    }

    strcpy(rest, rest_);
    return receiver_id;
}

char *get_msg_body(char *rest) {
    char *body;
    if ((body = strtok(rest, "\n\0")) == NULL) {
        printf("get message body error\n");
        return NULL;
    }
    return body;
}

void handle_input() {
    char buffer[MAX_LEN];
    fgets(buffer, MAX_LEN, stdin);
    char *args;
    char *cmd = strtok_r(buffer, " \0", &args);

    if ((strcmp(cmd, "LIST") == 0) || (strcmp(cmd, "LIST\n") == 0)) {
        send_LIST();
    } else if (strcmp(cmd, "2ALL") == 0) {
        char *text;
        text = strtok(args, "\n\0");
        if (text == NULL) {
            printf("Bad args\n");
            exit(12);
        }
        send_ALL(text);
    } else if (strcmp(cmd, "2ONE") == 0) {
        int receiver_id;
        char *text;
        if ((receiver_id = get_receiver_id(args)) < 0 || !(text = get_msg_body(args))) {
            printf("2ONE error\n");
            exit(13);
        }
        send_ONE(receiver_id, text);
    } else if ((strcmp(cmd, "STOP") == 0) || (strcmp(cmd, "STOP\n") == 0)) {
        send_STOP();
    }
}

int main() {

    printf("[CLIENT] Started running...\n");

    signal(SIGINT, handle_sigint);
    atexit(delete_client_queue);


    key_t key_s = ftok(home_path(), ID);
    if (key_s == -1) {
        printf("key generate error\n");
        return 1;
    }

    server_queue = msgget(key_s, 0);
    if (server_queue == -1) {
        printf("getting access to server queue error\n");
        return 2;
    }

    key_t key_c = ftok(home_path(), getpid());
    if (key_c == -1) {
        printf("key generate error\n");
        return 3;
    }

    client_queue = msgget(key_c, IPC_CREAT | IPC_EXCL | 0666);
    if (client_queue == -1) {
        printf("client queue create error\n");
        return 4;
    }

    printf("[CLIENT] My queue id: %d\n", client_queue);

    init();
    run();

    return 0;
}

