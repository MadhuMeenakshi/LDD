#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SIZE 100

struct message
{
    long msg_type;
    char msg_text[SIZE];
};

int main()
{
    key_t key;
    int msgid;
    struct message msg;

    key = ftok("msgqueue", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);

    while(1)
    {
        msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0);

        if(strcmp(msg.msg_text, "exit") == 0)
            break;

        printf("Received : %s\n", msg.msg_text);
    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
