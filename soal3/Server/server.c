#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void wFile(int socketFd) { 
    int n;
    FILE *fp;
    char *fName = "hartakarun.zip";
    char buffer[SIZE];

    fp = fopen(fName, "w");
    while (1) {
        n = recv(socketFd, buffer, SIZE, 0);
        if (n <= 0) {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int socketFd, new_sock;
    struct sockaddr_in serverAdd, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        perror("~Error in socket");
        exit(1);
    }
    printf("~Server socket created successfully.\n");

    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = port;
    serverAdd.sin_addr.s_addr = inet_addr(ip);

    e = bind(socketFd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (e < 0) {
        perror("~Error in bind");
        exit(1);
    }
    printf("~Binding successfull.\n");

    if (listen(socketFd, 10) == 0) {
        printf("~Listening....\n");
    }
    else{
        perror("~Error in listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(socketFd, (struct sockaddr *)&new_addr, &addr_size);
    wFile(new_sock);
    printf("~Data written in the file successfully.\n");

    return 0;
}
