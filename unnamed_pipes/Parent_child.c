#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 100

int main()
{
    int p2c[2], c2p[2];
    char buffer[SIZE];
    char response[SIZE];

    // Create two pipes
    if (pipe(p2c) == -1 || pipe(c2p) == -1)
    {
        perror("Pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Child Process

        close(p2c[1]); // Close write end of parent->child pipe
        close(c2p[0]); // Close read end of child->parent pipe

        while (1)
        {
            memset(buffer, 0, SIZE);

            read(p2c[0], buffer, SIZE);

            if (strcmp(buffer, "exit") == 0)
                break;

            printf("Child received: %s\n", buffer);

            snprintf(response, sizeof(response), "Processed: %s", buffer);

            write(c2p[1], response, strlen(response) + 1);
        }

        close(p2c[0]);
        close(c2p[1]);
    }
    else
    {
        // Parent Process

        close(p2c[0]); // Close read end
        close(c2p[1]); // Close write end

        while (1)
        {
            printf("\nEnter command: ");
            fgets(buffer, SIZE, stdin);

            buffer[strcspn(buffer, "\n")] = '\0';

            write(p2c[1], buffer, strlen(buffer) + 1);

            if (strcmp(buffer, "exit") == 0)
                break;

            memset(response, 0, SIZE);

            read(c2p[0], response, SIZE);

            printf("Response from child: %s\n", response);
        }

        close(p2c[1]);
        close(c2p[0]);

        wait(NULL);
    }

    return 0;
}
