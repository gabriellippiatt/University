#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_CONNECTIONS 79

// for use with strtol
char* p;
int failed;

struct AirportStructure {
    char* id;
    char* mapperPort;
    char** airports;
    int inputFD;
    int airportCounter;
};

// checks to see if the port is valid
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
            fprintf(stderr, "Invalid mapper port\n");
            exit(2);
        }
    } else {
        fprintf(stderr, "Invalid mapper port\n");
        exit(2);
    }
}

// checks the given airports to see if a mapper is required
void check_airports(int argc, char** argv) {
    int digit = 0;

    for (int i = 3; i < argc; i++) {
        int len = strlen(argv[i]);
        for (int j = 0; j < len; j++) {
            if (isdigit(argv[i][j]) == 0) {
                digit = 1;
                break;
            }
        }
    }
    if (digit == 1) {
        fprintf(stderr, "Mapper required\n");
        exit(3);
    }
}

// checks the arguments for errors
void arg_checker(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: roc2310 id mapper {airports}\n");
        exit(1);
    }
    if (strcmp(argv[2], "-") != 0) {
        invalid_port(argv[2]);
    } else {
        check_airports(argc, argv);
    }
}

// saves the arguments
void arg_save(int argc, char** argv, struct AirportStructure* airport) {
    airport->id = (char*)calloc(1000, sizeof(char));
    airport->mapperPort = (char*)calloc(1000, sizeof(char));
    airport->airportCounter = (argc - 3);
    strcpy(airport->id, argv[1]);
    strcpy(airport->mapperPort, argv[2]);

    for (int i = 3; i < argc; i++) {
        strcpy(airport->airports[i - 3], argv[i]);
    }
}

// sets up the client for the mapper
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
        fprintf(stderr, "Failed to connect to mapper\n");
        exit(4);
    }
    char buffer[1000];
    memset(buffer, 0, 1000);
    char* id = (char*)calloc(1000, sizeof(char));
    char* airportBadge = (char*)calloc(1000, sizeof(char));
    airportBadge[0] = '?';
    for (int i = 0; i < airport->airportCounter; i++) {
        strcpy(id, airport->airports[i]);
        for (int i = 0; i < strlen(id); i++) {
            airportBadge[i + 1] = id[i];
        }
        airportBadge[strlen(id) + 1] = '\n'; 
        
        send(cli, airportBadge, strlen(airportBadge), 0);

        recv(cli, buffer, 1000, 0);
        if (buffer[0] == '\0') {
            fprintf(stderr, "No map entry for destination\n");
            exit(5);
        }
        int bufferLen = strlen(buffer);
        char* recvMsg = (char*)calloc(1000, sizeof(char));
        for (int i = 0; i < bufferLen - 1; i++) {
            recvMsg[i] = buffer[i];
        }
        if (recvMsg[0] == ';') {
            fprintf(stderr, "No map entry for destination\n");
            exit(5);
        }
        strcpy(airport->airports[i], recvMsg);
    }
    close(cli);
}

// sets up the client for the control
void control_client_setup(char* airportPort, 
        struct AirportStructure* airport) {
    struct addrinfo* clientStruct = 0;
    struct addrinfo clientHints;
    memset(&clientHints, 0, sizeof(struct addrinfo));
    clientHints.ai_family = AF_INET; // IPv4  for generic could use AF_UNSPEC
    clientHints.ai_socktype = SOCK_STREAM;
    clientHints.ai_flags = AI_PASSIVE;  // Because we want to bind with it    
    getaddrinfo("localhost", airportPort, 
            &clientHints, &clientStruct);
        
    // create a socket and bind it to a port
    int cli = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    //int failed = 0;

    if (connect(cli, clientStruct->ai_addr, clientStruct->ai_addrlen) == -1) {
        failed = 1;
    } else {
        char buffer[1000];
        memset(buffer, 0, 1000);
        char* id = (char*)calloc(1000, sizeof(char));
        strcpy(id, airport->id);
        int idLen = strlen(airport->id);
        id[idLen] = '\n';
        send(cli, id, strlen(id), 0);
        recv(cli, buffer, sizeof(buffer), 0);
        
        int bufferLen = strlen(buffer);
        char* recvMsg = (char*)calloc(1000, sizeof(char));

        for (int i = 0; i < bufferLen; i++) {
            recvMsg[i] = buffer[i];
        }

        fprintf(stdout, "%s", recvMsg);
        fflush(stdout);
    }
    close(cli);
}

// main function
int main(int argc, char** argv) {
    struct AirportStructure* airport;
    airport = (struct AirportStructure*)
            calloc(1, sizeof(struct AirportStructure));
    airport->airports = (char**) calloc(1000, sizeof(char*));
    // calloc the 2D array
    for (int i = 0; i < 100; i++) {
        airport->airports[i] = (char*) calloc(1000, sizeof(char));
    }
    failed = 0;
    arg_checker(argc, argv);
    arg_save(argc, argv, airport);
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
    // Which port did we get?
    struct sockaddr_in ad;
    memset(&ad, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(struct sockaddr_in);
    if (getsockname(serv, (struct sockaddr*)&ad, &len)) {
        perror("sockname");
        return 4;
    }
    if (listen(serv, MAX_CONNECTIONS)) {
        perror("Listen");
        return 4;
    }
    if (strcmp(airport->mapperPort, "-") != 0) {
        mapper_client_setup(airport);
    }
    if (argc > 3) {
        for (int i = 0; i < airport->airportCounter; i++) {
            control_client_setup(airport->airports[i], airport);
        }
    }
    if (failed == 1) {
        fprintf(stderr, "Failed to connect to at least one destination\n");
        exit(6);
    }
    return 0;
}