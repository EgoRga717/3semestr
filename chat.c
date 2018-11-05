#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define N 100

int main(int argc)
{
    int pid;
    int fd[2];
    const char* out;
    const char* in;
    
    // Область жизни переменных нужно делать как можно меньше:
    // строка s у вас используется только отправки сообщений в родителе ... ну вот там её и следует объявить
    // resstring только в ребенке для приема текста и вывода на экран ... там и объявляем
    // Также из названия переменных сложно понять их предназвачение. Таких ситуаций стоит избегать и давать более "говорящие" названия.
    char s[N];
    char resstring[N];
    
    if (argc == 1)
    {
        out = "a.fifo";
        in = "b.fifo";
    }
    else
    {
        out = "b.fifo";
        in = "a.fifo";
    }
    umask(0);
    if(argc == 1)
    {
        if(mknod(out, S_IFIFO | 0666, 0) < 0)
        {
            printf("(%d): Can't create FIFO\n", argc);
            exit(-1);
        }
        if(mknod(in, S_IFIFO | 0666, 0) < 0)
        {
            printf("(%d): Can't create FIFO\n", argc);
            exit(-1);
        }
    }
    pid = fork();
    if(pid > 0) //Запись, родитель
    {
        fd[0] = open(out, O_WRONLY);
        while(1)
        {
            fgets(s, N * sizeof(char), stdin);
            write(fd[0], s, strlen(s) + 1);
        }
    }
    else //Чтение, ребенок
    {
        fd[1] = open(in, O_RDONLY);
        while(1)
        {
            read(fd[1], resstring, N);
            printf("%s", resstring);
        }
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}
