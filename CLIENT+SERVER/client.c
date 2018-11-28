//КЛИЕНТ

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
    int msqid;

    char pathname[] = "client.c";

    key_t key;

    int len, maxlen;

    struct mymsgbuf
    {
        long type;
        int id;
        int a;
        int b;
    } mybuf;

    struct mymsgbuf2
    {
        long id;
        int result;
    } mybuf2;


    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    len = sizeof(mybuf);
    mybuf.id = getpid();
    mybuf.type = 1;
    scanf("%d", &mybuf.a);
    scanf("%d", &mybuf.b);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    maxlen = sizeof(mybuf2);
    if( len = msgrcv(msqid, (struct msgbuf *) &mybuf2, maxlen, mybuf.id, 0) < 0){
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
    printf("RESULT: %d\n", mybuf2.result);

    return 0;
}
