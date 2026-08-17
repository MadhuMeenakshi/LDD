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
    int sockfd;
    struct sockaddr_un addr;
    char buffer[SIZE];

    sockfd = socket(AF_UNIX,SOCK_STREAM,0);

    if(sockfd<0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr,0,sizeof(addr));
    addr.sun_family=AF_UNIX;
    strcpy(addr.sun_path,SOCKET_PATH);

    connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));

    while(1)
    {
        printf("Enter Message : ");
        fgets(buffer,SIZE,stdin);

        buffer[strcspn(buffer,"\n")]='\0';

        write(sockfd,buffer,strlen(buffer)+1);

        if(strcmp(buffer,"exit")==0)
            break;

        read(sockfd,buffer,SIZE);

        printf("Server : %s\n",buffer);
    }

    close(sockfd);

    return 0;
}
