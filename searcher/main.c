#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void find(struct dirent* current_dir, int deep, char name[256], char way[1024])
{
    DIR *p;
    struct dirent* now_file;
    int plusSize;
    int waySize;
    int i;
    if(deep > 0)
    {
        strcat(way, "/");
        strcat(way, current_dir->d_name);
        plusSize = strlen(current_dir->d_name) + 1;
        p = opendir(way);
        if(deep >= 0)
            while((now_file = readdir(p)) != NULL)
                if(strcmp(now_file->d_name, ".") != 0 && strcmp(now_file->d_name, "..") != 0)
                {
                    if(now_file->d_type == DT_DIR)
                        find(now_file, deep - 1, name, way);
                    if(strcmp(now_file->d_name, name) == 0)
                        printf("%s/%s\n", way, name);
                }
        waySize = strlen(way);
        for(i = waySize - 1; i >= waySize - plusSize; --i)
            way[i] = '\0';
        closedir(p);
    }
}

int main(int argc, char *argv[])
{
    DIR* p;
    struct dirent* q;
    int max_deep, i;
    char search_name[256];
    char start_dir[256];
    char way[1024];
    for(i = 0; i < 1024; ++i)
        way[i] = '\0';
    strcat(way, ".");
    scanf("%s%s%d", &search_name, &start_dir, &max_deep);
    p = opendir("./");
    while((q = readdir(p)) != NULL){
        if(strcmp(q->d_name, start_dir) == 0)
            find(q, max_deep, search_name, way);
    }
    closedir(p);
    return 0;
}
