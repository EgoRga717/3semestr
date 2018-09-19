#include <stdio.h>
#include <malloc.h>

void GenerateString (int n, char* string)
{
    int i, size;
    char* left;
    char* right;
    size = 1 << n;
    if(n != 1)
    {
        left = (char*)malloc((size / 2) * sizeof(char));
        right = (char*)malloc((size / 2) * sizeof(char));
        for(i = 0; i < size / 2 - 1; ++i)
        {
            left[i] = string[i];
            right[i] = string[i + size / 2];
        }
        --n;
        GenerateString(n, left);
        GenerateString(n, right);
        for(i = 0; i < size / 2 - 1; ++ i)
        {
            string[i] = left[i];
            string[i + size / 2] = right[i];
        }
        string[size / 2 - 1] = 'a' + n;
        free(left);
        free(right);
    }
    else
        string[0] = 'a';
}

int main()
{
    int n, i;
    int size;
    char* s;
    scanf("%d", &n);
    size = (1 << n) - 1;
    s = (char*)malloc(size * sizeof(char));
    GenerateString(n, s);
    for(i = 0; i < size; ++i)
        printf("%c", s[i]);
    printf("\n");
    free(s);
    return 0;
}
