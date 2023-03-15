//
// Created by ppjas on 6/1/2022.
//


#include "com.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

client *clientsList[clientsMaximum] = {NULL};
int cliCounter = 0;


int getOpp(int index) {
    return index % 2 == 0 ? index + 1 : index - 1;
}


int appendClient(char *name, int fd) {
    for (int i = 0; i < clientsMaximum; i++) {
        if (clientsList[i] != NULL && strcmp(clientsList[i]->name, name) == 0) return -1;
    }

    int index = -1;
    for (int i = 0; i < clientsMaximum; i += 2) {
        if (clientsList[i] != NULL && clientsList[i + 1] == NULL) {
            index = i + 1;
            break;
        }
    }

    if (index == -1) {
        for (int i = 0; i < clientsMaximum; i++) {
            if (clientsList[i] == NULL) {
                index = i;
                break;
            }
        }
    }

    if (index != -1) {
        client *newClient = calloc(1, sizeof(client));
        newClient->name = calloc(MAX_MSG_LEN, sizeof(char));
        strcpy(newClient->name, name);
        newClient->fd = fd;
        newClient->available = true;

        clientsList[index] = newClient;
        cliCounter++;
    }
    return index;
}


int findClient(char *name) {
    for (int i = 0; i < clientsMaximum; i++) {
        if (clientsList[i] != NULL && strcmp(clientsList[i]->name, name) == 0) return i;
    }
    return -1;
}


void freeTheClient(int index) {
    free(clientsList[index]->name);
    free(clientsList[index]);
    clientsList[index] = NULL;
    cliCounter--;
    int opponent = getOpp(index);
    if (clientsList[getOpp(index)] != NULL) {
        printf("Removing opponent: %s\n", clientsList[opponent]->name);
        free(clientsList[opponent]->name);
        free(clientsList[opponent]);
        clientsList[opponent] = NULL;
        cliCounter--;
    }
}


void removeClient(char *name) {
    int index = -1;
    for (int i = 0; i < clientsMaximum; i++) {
        if (clientsList[i] != NULL && strcmp(clientsList[i]->name, name) == 0) index = i;
    }
    printf("Removing client %s . . .\n", name);
    freeTheClient(index);
}


void deleteIfClientNotAvailable() {
    for (int i = 0; i < clientsMaximum; i++) {
        if (clientsList[i] && !clientsList[i]->available) {
            printf("Not available :( \n");
            removeClient(clientsList[i]->name);
        }
    }
}


void sendPings() {
    for (int i = 0; i < clientsMaximum; i++) {
        if (clientsList[i]) {
            send(clientsList[i]->fd, "ping: ", MAX_MSG_LEN, 0);
            clientsList[i]->available = false;
        }
    }
}

_Noreturn void *ping() {
    while (true) {
        printf("PING\n");
        pthread_mutex_lock(&mutex);
        deleteIfClientNotAvailable();
        sendPings();
        pthread_mutex_unlock(&mutex);
        sleep(10);
    }
}

int setLocalSocket(char *socketPath) {
    int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sockFd == -1) {
        printf("Could not create local socket\n");
        exit(1);
    } else {
        printf("Local socket created \n");
    }
    struct sockaddr_un sockAddr;
    memset(&sockAddr, 0, sizeof(struct sockaddr_un));
    sockAddr.sun_family = AF_UNIX;
    strcpy(sockAddr.sun_path, socketPath);

    unlink(socketPath);

    if ((bind(sockFd, (struct sockaddr *) &sockAddr, sizeof(sockAddr))) == -1) {
        printf("Could not bind local socket\n");
        exit(1);
    }
    if ((listen(sockFd, clientsMaximum)) == -1) {
        printf("Listen on local socket failed\n");
        exit(1);
    }
    printf("Local socket fd: %d\n", sockFd);
    return sockFd;
}

int setNetworkSocket(char *port) {
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd == -1) {
        printf("Could not create inet socket\n");
        exit(1);
    } else {
        printf("Inet socket created\n");
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(atoi(port));
    if ((bind(sockFd, (struct sockaddr *) &sockAddr, sizeof(sockAddr))) == -1) {
        printf("Could not bind inet socket\n");
        exit(1);
    }

    if ((listen(sockFd, clientsMaximum)) == -1) {
        printf("Listen on inet socket failed\n");
        exit(1);
    }
    printf("Inet socket fd: %d\n", sockFd);
    return sockFd;
}


int checkMessages(int locSocket, int networkSocket) {
    struct pollfd *fds = calloc(2 + cliCounter, sizeof(struct pollfd));
    fds[0].fd = locSocket;
    fds[0].events = POLLIN;
    fds[1].fd = networkSocket;
    fds[1].events = POLLIN;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < cliCounter; i++) {
        fds[i + 2].fd = clientsList[i]->fd;
        fds[i + 2].events = POLLIN;
    }

    pthread_mutex_unlock(&mutex);
    poll(fds, cliCounter + 2, -1);
    int retval;
    for (int i = 0; i < cliCounter + 2; i++) {
        if (fds[i].revents & POLLIN) {
            retval = fds[i].fd;
            break;
        }
    }
    if (retval == locSocket || retval == networkSocket) {
        retval = accept(retval, NULL, NULL);
    }
    free(fds);
    return retval;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments!\n");
        exit(1);
    }
    srand(time(NULL));

    char *port = argv[1];
    char *socketPath = argv[2];
    int localSocket = setLocalSocket(socketPath);
    int networkSocket = setNetworkSocket(port);

    pthread_t t;
    pthread_create(&t, NULL, &ping, NULL);
    while (true) {
        int fd = checkMessages(localSocket, networkSocket);
        char buffer[MAX_MSG_LEN + 1];
        recv(fd, buffer, MAX_MSG_LEN, 0);

        printf("%s\n", buffer);
        char *command = strtok(buffer, ":");
        char *arg = strtok(NULL, ":");
        char *name = strtok(NULL, ":");

        pthread_mutex_lock(&mutex);
        if (strcmp(command, "add") == 0) {
            int index = appendClient(name, fd);
            if (index == -1) {
                send(fd, "add:name_taken", MAX_MSG_LEN, 0);
                close(fd);
            } else if (index % 2 == 0) {
                send(fd, "add:no_enemy", MAX_MSG_LEN, 0);
            } else {
                int random_num = rand() % 100;
                int first, second;
                if (random_num % 2 == 0) {
                    first = index;
                    second = getOpp(index);
                } else {
                    second = index;
                    first = getOpp(index);
                }
                send(clientsList[first]->fd, "add:O",
                     MAX_MSG_LEN, 0);
                send(clientsList[second]->fd, "add:X",
                     MAX_MSG_LEN, 0);
            }
        }
        if (strcmp(command, "move") == 0) {
            int move = atoi(arg);
            int player = findClient(name);

            sprintf(buffer, "move:%d", move);
            send(clientsList[getOpp(player)]->fd, buffer, MAX_MSG_LEN,
                 0);
        }
        if (strcmp(command, "end") == 0) {
            removeClient(name);
        }
        if (strcmp(command, "pong") == 0) {
            int player = findClient(name);
            if (player != -1) clientsList[player]->available = 1;

        }
        pthread_mutex_unlock(&mutex);
    }
}
