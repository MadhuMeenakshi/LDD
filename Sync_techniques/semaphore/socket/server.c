#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mysocket"
#define SIZE 100

int main()
{
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[SIZE];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    unlink(SOCKET_PATH);

    memset(&addr,0,sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    bind(server_fd,(struct sockaddr *)&addr,sizeof(addr));

    listen(server_fd,5);

    printf("Waiting for client...\n");

    client_fd = accept(server_fd,NULL,NULL);

    printf("Client Connected\n");

    while(1)
    {
        memset(buffer,0,SIZE);

        read(client_fd,buffer,SIZE);

        if(strcmp(buffer,"exit")==0)
            break;

        printf("Client : %s\n",buffer);

        sprintf(buffer,"Server Received");

        write(client_fd,buffer,strlen(buffer)+1);
    }

    close(client_fd);
    close(server_fd);

    unlink(SOCKET_PATH);

    return 0;
}
