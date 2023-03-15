//
// Created by ppjas on 6/1/2022.
//


#include "com.h"

int serverSocket;
int isClientO;
char buffer[MAX_MSG_LEN + 1];
char *name, *command, *arg;
game_board gameBoard;
state currState = START;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int move(game_board *board, int position) {
    if (position < 0 || position > 9 || board->objects[position] != FREE) return 0;
    board->objects[position] = board->move ? O : X;
    board->move = !board->move;
    return 1;
}


void end() {
    char buffer[MAX_MSG_LEN + 1];
    sprintf(buffer, "end: :%s", name);
    send(serverSocket, buffer, MAX_MSG_LEN, 0);
    exit(0);
}


sign checkWinner(game_board *board) {
    sign column = FREE;

    for (int x = 0; x < 3; x++) {
        sign first = board->objects[x];
        sign second = board->objects[x + 3];
        sign third = board->objects[x + 6];
        if (first == second && first == third && first != FREE)
            column = first;
    }

    if (column != FREE) {
        return column;
    }

    sign row = FREE;
    for (int y = 0; y < 3; y++) {
        sign first = board->objects[3 * y];
        sign second = board->objects[3 * y + 1];
        sign third = board->objects[3 * y + 2];
        if (first == second && first == third && first != FREE) {
            row = first;
        }
    }

    if (row != FREE) {
        return row;
    }

    sign lowDiag = FREE;
    sign first = board->objects[0];
    sign second = board->objects[4];
    sign third = board->objects[8];

    if (first == second && first == third && first != FREE) { lowDiag = first; }
    if (lowDiag != FREE) { return lowDiag; }
    sign uppDiag = FREE;
    first = board->objects[2];
    second = board->objects[4];
    third = board->objects[6];
    if (first == second && first == third && first != FREE) { uppDiag = first; }
    return uppDiag;
}


void gameStatus() {
    bool win = false;
    sign winner = checkWinner(&gameBoard);

    if (winner != FREE) {
        if ((isClientO && winner == O) || (!isClientO && winner == X)) printf("Mission complete ! ! ! \n");
        else printf("Mission failed :( \n");
        win = true;
    }
    bool draw = true;
    for (int i = 0; i < 9; i++) {
        if (gameBoard.objects[i] == FREE) {
            draw = false;
            break;
        }
    }
    if (draw && !win) printf("It's a draw!\n");
    if (win || draw) currState = QUIT;
}


void parseCommand(char *msg) {
    command = strtok(msg, ":");
    arg = strtok(NULL, ":");
}


game_board createBoard() {
    game_board board = {1, {FREE}};
    return board;
}


void draw() {
    char symbol;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (gameBoard.objects[y * 3 + x] == FREE) symbol = y * 3 + x + 1 + '0';
            else if (gameBoard.objects[y * 3 + x] == O) symbol = 'O';
            else symbol = 'X';
            printf("  %c  ", symbol);
        }
        printf("\n_________________________\n");
    }
}


void playGame() {
    while (true) {
        if (currState == START) {
            if (strcmp(arg, "name_taken") == 0) exit(1);
            else if (strcmp(arg, "no_enemy") == 0) currState = WAIT_FOR_OPPONENT;
            else {
                gameBoard = createBoard();
                isClientO = arg[0] == 'O';
                currState = isClientO ? MOVE : WAIT_FOR_MOVE;
            }
        } else if (currState == WAIT_FOR_OPPONENT) {

            pthread_mutex_lock(&mutex);
            while (currState != START && currState != QUIT) pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);

            gameBoard = createBoard();
            isClientO = arg[0] == 'O';
            currState = isClientO ? MOVE : WAIT_FOR_MOVE;
        } else if (currState == WAIT_FOR_MOVE) {
            printf("Waiting for rivals move\n");

            pthread_mutex_lock(&mutex);
            while (currState != OPPONENT_MOVE && currState != QUIT) {
                pthread_cond_wait(&cond, &mutex);
            }
            pthread_mutex_unlock(&mutex);
        } else if (currState == OPPONENT_MOVE) {
            int pos = atoi(arg);
            move(&gameBoard, pos);
            gameStatus();
            if (currState != QUIT) {
                currState = MOVE;
            }
        } else if (currState == MOVE) {
            draw();
            int pos;
            do {
                printf("Next move (%c): ", isClientO ? 'O' : 'X');
                scanf("%d", &pos);
                pos--;
            } while (!move(&gameBoard, pos));
            draw();
            char buffer[MAX_MSG_LEN + 1];
            sprintf(buffer, "move:%d:%s", pos, name);
            send(serverSocket, buffer, MAX_MSG_LEN, 0);
            gameStatus();
            if (currState != QUIT) {
                currState = WAIT_FOR_MOVE;
            }
        } else if (currState == QUIT) end();
    }
}


void connectLocal(char *path) {
    serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, path);
    if (connect(serverSocket, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
        printf("Error while connecting to LOCAL socket (%s)\n", strerror(errno));
        exit(1);
    }
}


void connectInet(char *port) {
    struct addrinfo *info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo("localhost", port, &hints, &info);
    serverSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

    if (connect(serverSocket, info->ai_addr, info->ai_addrlen) == -1) {
        printf("Error while connecting to INET socket (%s)\n", strerror(errno));
        exit(1);
    }
    freeaddrinfo(info);
}


void listenServer() {
    int threadRunning = 0;
    while (1) {
        recv(serverSocket, buffer, MAX_MSG_LEN, 0);
        parseCommand(buffer);
        pthread_mutex_lock(&mutex);
        if (strcmp(command, "add") == 0) {
            currState = START;
            if (!threadRunning) {
                pthread_t t;
                pthread_create(&t, NULL, (void *(*)(void *)) playGame, NULL);
                threadRunning = 1;
            }
        } else if (strcmp(command, "move") == 0) {
            currState = OPPONENT_MOVE;
        } else if (strcmp(command, "end") == 0) {
            currState = QUIT;
            exit(0);
        } else if (strcmp(command, "ping") == 0) {
            sprintf(buffer, "pong: :%s", name);
            send(serverSocket, buffer, MAX_MSG_LEN, 0);
        }
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    name = argv[1];
    signal(SIGINT, end);

    if (strcmp(argv[2], "unix") == 0) {
        char *path = argv[3];
        connectLocal(path);
    } else if (strcmp(argv[2], "inet") == 0) {
        char *port = argv[3];
        connectInet(port);
    } else {
        printf("Wrong method - choose [inet] or [unix]!\n");
        exit(1);
    }

    char msg[MAX_MSG_LEN];
    sprintf(msg, "add: :%s", name);
    send(serverSocket, msg, MAX_MSG_LEN, 0);

    listenServer();
    return 0;
}
