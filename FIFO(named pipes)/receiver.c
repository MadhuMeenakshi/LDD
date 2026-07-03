#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"
#define SIZE 100

int main()
{
    char buffer[SIZE];

    // Create FIFO
    mkfifo(FIFO_NAME, 0666);

    // Open FIFO for reading
    int fd = open(FIFO_NAME, O_RDONLY);

    if(fd < 0)
    {
        perror("open");
        return -1;
    }

    while(1)
    {
        int n = read(fd, buffer, SIZE);

        if(n > 0)
        {
            buffer[n] = '\0';

            if(strcmp(buffer, "exit") == 0)
                break;

            printf("Received : %s\n", buffer);
        }
    }

    close(fd);

    unlink(FIFO_NAME);

    return 0;
}
