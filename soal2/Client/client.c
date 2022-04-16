#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
 
int startClient(struct sockaddr_in serv_addr)
{
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    memset(&serv_addr, '0', sizeof(serv_addr));
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return fd;
}
 
int main()
{
    struct sockaddr_in serv_addr;
    int fd = startClient(serv_addr), valread;
    while (1)
    {
        fflush(stdin);
        char response_buffer[1024] = {0}, buffer[100] = {0};
        read(fd, response_buffer, 1024);
        // Case if server sends a reinput
        if (!strcmp(response_buffer, "re_inp"))
        {
            printf("%s\n", response_buffer);
            memset(response_buffer, 0, strlen(response_buffer));
            read(fd, response_buffer, 1024);
            printf("%s\n", response_buffer);
            memset(response_buffer, 0, strlen(response_buffer));
            read(fd, response_buffer, 1024);
        }
        printf("%s\n", response_buffer);
        fgets(buffer, 200, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        // scanf("%s", buffer);
        send(fd, buffer, 100, 0);
    }
 
    return 0;
}
