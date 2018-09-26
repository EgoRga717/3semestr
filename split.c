#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBUKAV 1000

void Split (char* string, char del)
{
    int i = 0;
    while((string[i] != '\n') && (string[i] != '\0'))
    {
        if(string[i] == del)
            printf("\n");
        else
            printf("%c", string[i]);
        ++i;
    }
}

int main()
{
    char s[MAXBUKAV];
    char d;
    printf("Vvedite razdelyayushiy simvol:\n");
    scanf("%c", &d);
    printf("Vvedite stroku, kak vvedyote, nazhmite ENTER:\n");
    scanf("%s", s);
    Split(s, d);
    return 0;
}
