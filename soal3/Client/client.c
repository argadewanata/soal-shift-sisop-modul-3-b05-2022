#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void sFile(FILE *fp, int sockFd) {
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL) {
        if (send(sockFd, data, sizeof(data), 0) == -1) {
            mError("~ Error in sending file.");
            exit(1);
        }
        bZero(data, SIZE);
    }
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockFd;
    struct sockaddr_in serverAdd;
    FILE *fp;
    char *fName = "hartakarun.zip";

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        mError("~ Error in socket");
        exit(1);
    }
    printf("~ Server socket created successfull\n");

    serverAdd.sin_port = port;
    serverAdd.sin_addr.s_addr = inet_addr(ip);
    serverAdd.sin_family = AF_INET;


    e = connect(sockFd, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
    if (e == -1) {
        mError("~ Error in socket");
        exit(1);
    }
    printf("~ Connected to Server\n");

    char command[100];
    scanf("%s", command);
    if (strcmp(command, "send hartakarun.zip") == 0) {
        // scanf("%s", fName);

        fp = fopen(fName, "r");
        if (fp == NULL) {
            mError("~ Error in reading file.");
            exit(1);
        }

        sFile(fp, sockFd);
        printf("~ File data sent successfully.\n");

        printf("~ Closing the connection.\n");
        close(sockFd);
    }
    return 0;
}
