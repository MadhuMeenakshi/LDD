#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;

int value = 0;

void *reader(void *arg)
{
    int id = *(int *)arg;

    pthread_rwlock_rdlock(&rwlock);

    printf("Reader %d : Value = %d\n", id, value);

    sleep(2);

    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

void *writer(void *arg)
{
    pthread_rwlock_wrlock(&rwlock);

    value++;

    printf("Writer Updated Value = %d\n", value);

    sleep(2);

    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

int main()
{
    pthread_t r1,r2,r3,w1;

    int id1=1,id2=2,id3=3;

    pthread_rwlock_init(&rwlock,NULL);

    pthread_create(&r1,NULL,reader,&id1);
    pthread_create(&r2,NULL,reader,&id2);
    pthread_create(&r3,NULL,reader,&id3);

    sleep(1);

    pthread_create(&w1,NULL,writer,NULL);

    pthread_join(r1,NULL);
    pthread_join(r2,NULL);
    pthread_join(r3,NULL);
    pthread_join(w1,NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
