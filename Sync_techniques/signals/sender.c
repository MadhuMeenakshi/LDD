#include <stdio.h>
#include <signal.h>

int main()
{
    int pid;

    printf("Enter Receiver PID : ");
    scanf("%d",&pid);

    kill(pid,SIGUSR1);

    printf("Signal Sent\n");

    return 0;
}
