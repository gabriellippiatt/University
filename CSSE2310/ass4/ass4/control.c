#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

#define MAX_CONNECTIONS 79
// used for multi threading
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// for use with strtol
char* p;

struct AirportStructure {
    char* id;
    char* info;
    char* mapperPort;
    char* controlPort;
    char** log;
    char* airportBadge;
    int planeCounter;
};

struct ThreadStructure {
    struct AirportStructure* airportCopy;
    int inputFD;
};

// checks the ID and info for invalid characters
void invalid_chars(char* id, char* info) {
    int idLen = strlen(id);
    int infoLen = strlen(info);
    for (int i = 0; i < idLen; i++) {
        if (id[i] == '\n' || id[i] == '\r' || id[i] == ':') {
            fprintf(stderr, "Invalid char in parameter\n");
            exit(2);
        }
    }
    for (int i = 0; i < infoLen; i++) {
        if (info[i] == '\n' || info[i] == '\r' || info[i] == ':') {
            fprintf(stderr, "Invalid char in parameter\n");
            exit(2);
        }
    }
}

// checks if the mapper port is valid
void invalid_port(char* port) {
    int len = strlen(port);
    int digit = 0;

    for (int i = 0; i < len; i++) {
        if (isdigit(port[i]) == 0) {
            digit = 1;
            break;
        }
    }
    if (digit == 0) {
        int portNum = strtol(port, &p, 10);
        if (portNum > 65534 || portNum < 1) {
            fprintf(stderr, "Invalid port\n");
            exit(3);
        }
    } else {
        fprintf(stderr, "Invalid port\n");
        exit(3);
    }
}

// checks the arguments for errors
void arg_checker(int argc, char** argv) {
    if ((argc != 3) && argc != 4) {
        fprintf(stderr, "Usage: control2310 id info [mapper]\n");
        exit(1);
    }
    invalid_chars(argv[1], argv[2]);
    if (argc == 4) {
        invalid_port(argv[3]);
    }
}

// save the arguements in a struct
void arg_save(int port, int argc, char** argv, 
        struct AirportStructure* airport) {
    printf("%u\n", port);
    fflush(stdout);
    airport->planeCounter = 0;
    sprintf(airport->controlPort, "%d", port);
    airport->mapperPort = (char*)calloc(1000, sizeof(char));
    airport->airportBadge = (char*)calloc(1000, sizeof(char));
    airport->id = (char*)calloc(1000, sizeof(char));
    airport->info = (char*)calloc(1000, sizeof(char));
    strcpy(airport->id, argv[1]);
    strcpy(airport->info, argv[2]);

    if (argc == 4) {
        strcpy(airport->mapperPort, argv[3]);
    }

    int idLen = strlen(airport->id);
    int controlPortLen = strlen(airport->controlPort);
    int bigLen = (2 + idLen + controlPortLen);

    for (int i = 0; i < bigLen; i++) {
        if (i == 0) {
            airport->airportBadge[0] = '!';
        } else if (i <= idLen) {
            airport->airportBadge[i] = airport->id[i - 1];
        } else if (i == (idLen + 1)) {
            airport->airportBadge[i] = ':';
        } else if (i > (idLen + 1)) {
            airport->airportBadge[i] = airport->controlPort[i - (idLen + 2)];
        }
    }

    int badgeLen = strlen(airport->airportBadge);
    airport->airportBadge[badgeLen] = '\n';
}

// displays the recorded logs of planes
void display_log(FILE* write, struct AirportStructure* airport) {
    char temp[1000];

    // storing strings in the lexicographical order
    for (int i = 0; i < airport->planeCounter; ++i) {
        for (int j = i + 1; j < airport->planeCounter; ++j) {

            // swapping strings if they are not in the lexicographical order
            if (strcmp(airport->log[i], airport->log[j]) > 0) {
                strcpy(temp, airport->log[i]);
                strcpy(airport->log[i], airport->log[j]);
                strcpy(airport->log[j], temp);
            }
        }
    }
    for (int i = 0; i < airport->planeCounter; i++) {
        fprintf(write, "%s", airport->log[i]);
        fflush(write);
    }

    fprintf(write, ".\n");
    fflush(write);
}

// checks the messages recieved by ROC
void* response_checker(void* v) {
    struct ThreadStructure* thread = (struct ThreadStructure*)v;
    FILE* write = fdopen(thread->inputFD, "w");
    int reader = dup(thread->inputFD);
    FILE* read = fdopen(reader, "r");

    int len = strlen(thread->airportCopy->info);
    char* send = (char*)calloc(1000, sizeof(char));
    strcpy(send, thread->airportCopy->info);
    send[len] = '\n';
    
    while (1) {
        char fileRead[1000];
        memset(fileRead, 0, 1000);
        
        if (fgets(fileRead, 1000, read) != NULL) {
            pthread_mutex_lock(&mutex);
            if (strlen(fileRead) > 0) {
                if (fileRead[0] == 'l' && fileRead[1] == 'o' 
                        && fileRead[2] == 'g') {
                    display_log(write, thread->airportCopy);
                } else {
                    strcpy(
                            thread->airportCopy->
                            log[thread->airportCopy->planeCounter], fileRead);
                    thread->airportCopy->planeCounter += 1;
                    fprintf(write, "%s", send);
                    fflush(write);
                }
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    free(thread);
    return 0;
}

// sets up the client
void mapper_client_setup(struct AirportStructure* airport) {
    struct addrinfo* clientStruct = 0;
    struct addrinfo clientHints;
    memset(&clientHints, 0, sizeof(struct addrinfo));
    clientHints.ai_family = AF_INET; // IPv4  for generic could use AF_UNSPEC
    clientHints.ai_socktype = SOCK_STREAM;
    clientHints.ai_flags = AI_PASSIVE;  // Because we want to bind with it    
    getaddrinfo("localhost", airport->mapperPort, 
            &clientHints, &clientStruct);
    // create a socket and bind it to a port
    int cli = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    if (connect(cli, clientStruct->ai_addr, clientStruct->ai_addrlen) == -1) {
        fprintf(stderr, "Can not connect to map\n");
        exit(4);
    }
    send(cli, airport->airportBadge, strlen(airport->airportBadge), 0);
    close(cli);
}

// main function
int main(int argc, char** argv) {
    struct AirportStructure* airport;
    airport = (struct AirportStructure*)
            calloc(1, sizeof(struct AirportStructure));    
    airport->controlPort = (char*)calloc(1000, sizeof(char));
    airport->log = (char**) calloc(1000, sizeof(char*));
    for (int i = 0; i < 100; i++) {
        airport->log[i] = (char*) calloc(1000, sizeof(char));
    }
    arg_checker(argc, argv);
    struct addrinfo* ai = 0;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        // IPv4  for generic could use AF_UNSPEC
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // Because we want to bind with it    
    getaddrinfo("localhost", 0, &hints, &ai);  // no particular port
    // create a socket and bind it to a port
    int serv = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    if (bind(serv, (struct sockaddr*)ai->ai_addr, sizeof(struct sockaddr))) {
        perror("Binding");
        return 3;
    }
    struct sockaddr_in ad;
    memset(&ad, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(struct sockaddr_in);
    if (getsockname(serv, (struct sockaddr*)&ad, &len)) {
        perror("sockname");
        return 4;
    }
    arg_save(ntohs(ad.sin_port), argc, argv, airport);
    if (listen(serv, MAX_CONNECTIONS)) {
        perror("Listen");
        return 4;
    }
    int connFD;
    if (argc == 4) {
        mapper_client_setup(airport);
    }
    while (connFD = accept(serv, 0, 0), connFD >= 0) {
        struct ThreadStructure* thread;
        thread = (struct ThreadStructure*)
                calloc(1, sizeof(struct ThreadStructure));
        pthread_t tid;
        thread->inputFD = connFD;
        thread->airportCopy = airport;
        pthread_create(&tid, NULL, response_checker, thread);
        pthread_detach(tid);
    }
    return 0;
}