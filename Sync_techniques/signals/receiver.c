#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
    printf("\nSignal %d Received\n",sig);
}

int main()
{
    signal(SIGUSR1,handler);

    printf("Receiver PID : %d\n",getpid());

    while(1);

    return 0;
}
