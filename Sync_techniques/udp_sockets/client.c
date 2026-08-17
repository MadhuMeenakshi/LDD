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

    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET,
              "127.0.0.1",
              &server_addr.sin_addr);

    while(1)
    {
        printf("Enter Message : ");

        fgets(buffer, SIZE, stdin);

        buffer[strcspn(buffer,"\n")] = '\0';

        sendto(sockfd,
               buffer,
               strlen(buffer)+1,
               0,
               (struct sockaddr *)&server_addr,
               sizeof(server_addr));

        if(strcmp(buffer,"exit")==0)
            break;

        recvfrom(sockfd,
                 buffer,
                 SIZE,
                 0,
                 NULL,
                 NULL);

        printf("Server : %s\n",buffer);
    }

    close(sockfd);

    return 0;
}
