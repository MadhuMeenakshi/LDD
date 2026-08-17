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

    struct sockaddr_in server_addr;

    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while(1)
    {
        printf("Enter Message : ");

        fgets(buffer, SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        write(sockfd, buffer, strlen(buffer)+1);

        if(strcmp(buffer, "exit") == 0)
            break;

        memset(buffer, 0, SIZE);

        read(sockfd, buffer, SIZE);

        printf("Server : %s\n", buffer);
    }

    close(sockfd);

    return 0;
}
