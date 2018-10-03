#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
	int i;
	printf("%d\n", argc);
	printf("\n\n");
	for(i = 0; argv[i] != NULL; ++i)
	{
		printf("%s\n", argv[i]);
	}
	printf("\n\n");
	for(i = 0; envp[i] != NULL; ++i)
	{
		printf("%s\n", envp[i]);
	}
	return 0;
}

