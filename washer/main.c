#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdarg.h>
#define N 3
#define TABLESIZE 10

int main(int argc)
{
    int pid;
    int semValue = 0;
    int fd[2];
    char s[N];
    char resstring[N];
    int wareSize[N];
    int washTime[N];
    int cleanTime[N];
    int i;
    int k = 0;
    int SZ, CT;

    int semid;

    char pathname[] = "main.c";

    key_t key;

    struct sembuf mybuf;

    //Initialization
    wareSize[0] = 1; washTime[0] = 1; cleanTime[0] = 1;
    wareSize[1] = 5; washTime[1] = 3; cleanTime[1] = 6;
    wareSize[2] = 3; washTime[2] = 5; cleanTime[2] = 2;

    if((key = ftok(pathname,0)) < 0){
    printf("Can\'t generate key\n");
    exit(-1);
    }

    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get semid\n");
    exit(-1);
    }

    if(semop(semid, &mybuf, 1) < 0){
    printf("Can\'t wait for condition\n");
    exit(-1);
    }

    umask(0);
    if(mknod("a.fifo", S_IFIFO | 0666, 0) < 0)
    {
        printf("(%d): Can't create FIFO\n", argc);
        exit(-1);
    }
    for(i = 0; i < TABLESIZE; ++i)//Значение семафора 50, что значит 50 свободного места на столе
    {
        //A(1)
        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;
        semop(semid, &mybuf, 1);
    }
    pid = fork();
    if(pid > 0) //Мойщик, родитель
    {
        fd[0] = open("a.fifo", O_WRONLY);
        while(k < N)
        {
        printf("W:Taking a ware.\n");
        printf("W:Start washing...\n");
        sleep(washTime[k]);
        printf("W:End washing.\n");
        printf("W:Start putting ware № %d on the table...\n", k);
        for(i = 0; i < wareSize[k]; ++i)
        {
            //D(1)
            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;
            mybuf.sem_num = 0;
            semop(semid, &mybuf, 1);
        }
        sprintf(s, "%d", wareSize[k]);
        write(fd[0], s, 2);
        sprintf(s, "%d", wareSize[k]);
        write(fd[0], s, 2);
        printf("W:End putting ware № %d on the table.\n", k);
        ++k;
        }
        printf("W: I have done my work! Goodbye!\n");
    }
    else //Вытиратель, ребенок
    {
        fd[0] = open("a.fifo", O_RDONLY);
        while(k < N)
        {
            printf("C:Searching a ware.\n");
            read(fd[0], resstring, 2);
            SZ = atoi(resstring);//size of ware
            read(fd[0], resstring, 2);
            CT = atoi(resstring);//time for clean
            printf("C:I understand what is it.\n");
            printf("C:Taking a ware № %d from table...\n", k);
            for(i = 0; i < SZ; ++i)
            {
                //A(1)
                mybuf.sem_op = 1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 0;
                semop(semid, &mybuf, 1);
            }
            printf("C:End taking a ware № %d from table.\n", k);
            printf("C:Start washing...\n");
            sleep(CT);
            printf("C:End washing.\n");
            ++k;
        }
        printf("C: I have done my work! Goodbye!\n");
        close(fd[0]);
    }
    return 0;
}
