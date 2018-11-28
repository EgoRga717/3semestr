//СЕРВЕР
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#define MAXPROC 2//Максимально возможное число одновременно выполняющихся процессов

int main()
{
    int msqid;

    char pathname[] = "client.c";

    key_t key;

    int len, maxlen, f, i;

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
//Добавим семафор
    int semid;

    char pathname2[] = "server.c";

    key_t key2;

    struct sembuf mybuff;

    if((key2 = ftok(pathname2,0)) < 0){
    printf("Can\'t generate key\n");
    exit(-1);
    }

    if((semid = semget(key2, 1, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get semid\n");
    exit(-1);
    }

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    for(i = 0; i < MAXPROC; ++i)
    {
        //A(1)
        mybuff.sem_op = 1;
        mybuff.sem_flg = 0;
        mybuff.sem_num = 0;
        semop(semid, &mybuff, 1);
    }
    while(1)
    {
        if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
            printf("Can\'t get msqid\n");
            exit(-1);
        }
        maxlen = sizeof(mybuf);
        if( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        //D(1)
        mybuff.sem_op = -1;
        mybuff.sem_flg = 0;
        mybuff.sem_num = 0;
        semop(semid, &mybuff, 1);

        f = fork();
        if(f == 0)
        {
            mybuf2.result = mybuf.a * mybuf.b;
            mybuf2.id = mybuf.id;
            printf("a * b = %d, id = %d\n", mybuf2.result, mybuf2.id);
            sleep(5);
            len = sizeof(mybuf2);
            if (msgsnd(msqid, (struct msgbuf *) &mybuf2, len, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
            //A(1)
            mybuff.sem_op = 1;
            mybuff.sem_flg = 0;
            mybuff.sem_num = 0;
            semop(semid, &mybuff, 1);
            exit(0);
        }
    }

    return 0;
} 
