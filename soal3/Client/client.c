#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void sFile(FILE *fp, int socketFd) {
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL) {
        if (send(socketFd, data, sizeof(data), 0) == -1) {
            perror("~Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int socketFd;
    struct sockaddr_in serverAdd;
    FILE *fp;
    char *fName = "hartakarun.zip";

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        perror("~Error in socket");
        exit(1);
    }
    printf("~Server socket created successfully.\n");

    serverAdd.sin_port = port;
    serverAdd.sin_addr.s_addr = inet_addr(ip);
    serverAdd.sin_family = AF_INET;


    e = connect(socketFd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (e == -1) {
        perror("~Error in socket");
        exit(1);
    }
    printf("~Connected to Server.\n");

    char comm[100];
    scanf("%s", comm);
    if (strcmp(comm, "send hartakarun.zip") == 0) {
        // scanf("%s", fName);

        fp = fopen(fName, "r");
        if (fp == NULL) {
            perror("~Error in reading file.");
            exit(1);
        }

        sFile(fp, socketFd);
        printf("~File data sent successfully.\n");

        printf("~Closing the connection.\n");
        close(socketFd);
    }
    return 0;
}
