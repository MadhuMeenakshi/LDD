#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    char buffer[SIZE];

    // Create Socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Server Address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen
    listen(server_fd, 5);

    printf("Server Waiting...\n");

    // Accept Client
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    printf("Client Connected\n");

    while(1)
    {
        memset(buffer, 0, SIZE);

        read(client_fd, buffer, SIZE);

        if(strcmp(buffer, "exit") == 0)
            break;

        printf("Client : %s\n", buffer);

        strcpy(buffer, "Message Received");

        write(client_fd, buffer, strlen(buffer)+1);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
