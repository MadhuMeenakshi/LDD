#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SIZE 100

int main()
{
    key_t key = ftok("semfile",65);

    int shmid = shmget(key,SIZE,0666|IPC_CREAT);
    int semid = semget(key,1,0666|IPC_CREAT);

    char *data = (char *)shmat(shmid,NULL,0);

    struct sembuf lock = {0,-1,0};
    struct sembuf unlock = {0,1,0};

    while(1)
    {
        semop(semid,&lock,1);

        if(strlen(data)>0)
        {
            printf("Received : %s\n",data);

            if(strcmp(data,"exit")==0)
            {
                semop(semid,&unlock,1);
                break;
            }

            data[0]='\0';
        }

        semop(semid,&unlock,1);

        sleep(1);
    }

    shmdt(data);

    shmctl(shmid,IPC_RMID,NULL);
    semctl(semid,0,IPC_RMID);

    return 0;
}
