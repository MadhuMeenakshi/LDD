#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

int data = 0;
int ready = 0;

void *producer(void *arg)
{
    printf("Producer: Enter a number: ");
    scanf("%d", &data);

    pthread_mutex_lock(&lock);

    ready = 1;

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&lock);

    return NULL;
}

void *consumer(void *arg)
{
    pthread_mutex_lock(&lock);

    while(ready == 0)
    {
        printf("Consumer: Waiting...\n");
        pthread_cond_wait(&cond, &lock);
    }

    printf("Consumer: Received %d\n", data);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, consumer, NULL);
    pthread_create(&t2, NULL, producer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
