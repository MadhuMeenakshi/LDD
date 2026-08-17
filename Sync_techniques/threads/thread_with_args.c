#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg)
{
    printf("Hello from Child Thread!\n");
    return NULL;
}

int main()
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, thread_function, NULL) != 0)
    {
        perror("pthread_create");
        return -1;
    }

    printf("Hello from Main Thread!\n");

    pthread_join(tid, NULL);

    printf("Child Thread Finished.\n");

    return 0;
}
