#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREADS 6

sem_t sem;

void *worker(void *arg)
{
    int id = *(int *)arg;

    printf("Thread %d is waiting...\n", id);

    // Wait (decrement semaphore)
    sem_wait(&sem);

    printf("Thread %d entered critical section.\n", id);

    // Simulate using the resource
    sleep(3);

    printf("Thread %d leaving critical section.\n", id);

    // Signal (increment semaphore)
    sem_post(&sem);

    return NULL;
}

int main()
{
    pthread_t tid[THREADS];
    int id[THREADS];

    // Initialize counting semaphore with count = 3
    sem_init(&sem, 0, 3);

    for(int i = 0; i < THREADS; i++)
    {
        id[i] = i + 1;
        pthread_create(&tid[i], NULL, worker, &id[i]);
    }

    for(int i = 0; i < THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&sem);

    return 0;
}
