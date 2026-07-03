#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "myfifo"
#define SIZE 100

int main()
{
    char buffer[SIZE];

    int fd = open(FIFO_NAME, O_WRONLY);

    if(fd < 0)
    {
        perror("open");
        return -1;
    }

    while(1)
    {
        printf("Enter message : ");
        fgets(buffer, SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        write(fd, buffer, strlen(buffer));

        if(strcmp(buffer, "exit") == 0)
            break;
    }

    close(fd);

    return 0;
}
