#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_CONNECTIONS 79

int main(int argc, char** argv) {
    struct addrinfo* ai = 0;
    struct addrinfo hints;
    memset(& hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;        // IPv4  for generic could use AF_UNSPEC
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;  // Because we want to bind with it    
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
    socklen_t len=sizeof(struct sockaddr_in);
    if (getsockname(serv, (struct sockaddr*)&ad, &len)) {
        perror("sockname");
        return 4;
    }
    printf("%u\n", ntohs(ad.sin_port));
    fflush(stdout);
    
    if (listen(serv, MAX_CONNECTIONS)) {     // allow up to 10 connection requests to queue
        perror("Listen");
        return 4;
    }
    
    int conn_fd;
  
    while (conn_fd = accept(serv, 0, 0), conn_fd >= 0) {    // change 0, 0 to get info about other end
        FILE* stream = fdopen(conn_fd, "w");
        int input_fd = dup(conn_fd);
        FILE* input_fp = fdopen(input_fd, "r");
        while(1) {
            char* response = (char*)calloc(100, sizeof(char));
            fgets(response, 100, input_fp);
            printf("%s", response);
            fflush(stdout);
            
        }
    }
    return 0;
}