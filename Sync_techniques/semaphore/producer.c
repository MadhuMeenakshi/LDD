#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SIZE 100

union semun
{
    int val;
};

int main()
{
    key_t key = ftok("semfile",65);

    int shmid = shmget(key,SIZE,0666|IPC_CREAT);
    int semid = semget(key,1,0666|IPC_CREAT);

    union semun arg;
    arg.val = 1;
    semctl(semid,0,SETVAL,arg);

    char *data = (char *)shmat(shmid,NULL,0);

    struct sembuf lock = {0,-1,0};
    struct sembuf unlock = {0,1,0};

    while(1)
    {
        semop(semid,&lock,1);

        printf("Enter message : ");
        fgets(data,SIZE,stdin);
        data[strcspn(data,"\n")] = '\0';

        semop(semid,&unlock,1);

        if(strcmp(data,"exit")==0)
            break;
    }

    shmdt(data);

    return 0;
}
