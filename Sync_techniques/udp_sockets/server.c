#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int sockfd;
    char buffer[SIZE];

    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server Waiting...\n");

    while(1)
    {
        memset(buffer, 0, SIZE);

        recvfrom(sockfd,
                 buffer,
                 SIZE,
                 0,
                 (struct sockaddr *)&client_addr,
                 &len);

        if(strcmp(buffer, "exit") == 0)
            break;

        printf("Client : %s\n", buffer);

        strcpy(buffer, "Message Received");

        sendto(sockfd,
               buffer,
               strlen(buffer)+1,
               0,
               (struct sockaddr *)&client_addr,
               len);
    }

    close(sockfd);

    return 0;
}
