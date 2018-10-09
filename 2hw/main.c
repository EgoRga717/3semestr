//У меня ошибка сегментирования, все никак не пойму где она и как ее исправить. 
//Потратил на эту прогу 8 часов, в общем, я надеюсь, вы поможете мне и укажите на ошибку

// Я не везде понимаю, почему написано именно так. Можно на семинаре обсудить. Вообще падения разные локализуют путем комментирования
// кусков кода ... сначала закомментировали все - работает, потом кусочек оставили - работает, еще один - ага, упало. Нашли.
// Вызывает сомнения:
// 1) (fgets(input, NUM, file) != '\0') && (fgets(input, NUM, file) != NULL)
// fgets возвращет char*, вы его сравниваете с '\0', т.е. char. Также вы считаваете сразу две строки, причем одна пишется поверх другой
// 2) на этапе 4 выведите массив argv, чтобы убедиться, что считывание корректно прошло
// 3) уберите пока все kill, чтобы убедиться, что без таймаутов все работает

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#define NUM 100 //Максимальное количество букв в строке
#define MAXSIZE 1000 //Максимальное количество строк в файле
#define MAXNAME 20 // Максимальное количество букв в названии программы

int main(int argc, char *argv[])
{
	char input[MAXSIZE];
	char *arv[MAXSIZE];
	char name[MAXNAME];
	int pid, pidd;
	int time;
	int arc;
	int i, j = 0;
	int etap = 0;
	FILE *file = fopen("com.txt", "r");
	if(file != NULL) //А вообще мы открыли файл?
	{
		while((fgets(input, NUM, file) != '\0') && (fgets(input, NUM, file) != NULL))
		{
			//Сейчас поэтапно в нужные переменные будем все записывать
			if(etap == 0) //Название программы
			{
				strcpy(name, input);
				++etap;
			}
			if(etap == 1) //Время, через которое запустится программа
			{
				time = atoi(input);
				++etap;
			}
			if(etap == 2) //А вот и argc подъехал
			{
				arc = atoi(input);
				++etap;
			}
			while(etap == 3) //Массив argv
			{
				if(j < arc)
				{
					strcpy(arv[j], input);
					++j;
				}
				else
				{
					++etap;
					j = 0;
				}
			}
			if(etap == 4) 
			{
				pid = fork();
				if(pid == 0)
				{
					pidd = fork();
					if(pidd == 0)//Будущий убийца своего родителя
					{
						sleep(5);
						kill(getppid(), 9);
					}
					else//Ни о чем не подозревающий процесс, который запускает программы с нужными аргументами
					{
					etap = -1; 
					sleep(time);
					execvp(name, arv);
					kill(getpid(), 9);
					}
					
				}
				else//Чистит массивы и обнуляет этап
				{
					etap = 0;
					memset(name, '\0', sizeof(char) * MAXNAME);
					memset(arv, '\0', sizeof(char) * NUM * arc);
				}
			}
			++i;	
		}	
	}
	else
		perror("File is empty!\n");
	fclose(file);
	return 0;
}

