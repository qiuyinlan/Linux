#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char *path = (argc > 1)?argv[1]:".";
    dir = opendir(path);
    if(dir == NULL){
        perror("opendir");
        return EXIT_FAILURE;
    }
    int opt;
    int show_all = 0, show_long = 0, recursive = 0, sort_time = 0;
    while ((opt = getopt(argc,argv,"alRtrsi")) != -1) {
        switch (opt) {
            case 'a':
                show_all = 1;
                break;
            case 'l':
                show_long = 1;
                break;
            case 'R':
                recursive = 1;
                break;
            case 't':
                sort_time = 1;
                break;
        // 处理其他参数
    }
    DIR *dir = opendir(path);
if (dir == NULL) {
    perror("opendir");
    return 1;
}

struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    // 如果有 -a 参数，显示所有文件（包括以 . 开头的文件）
    if (show_all || entry->d_name[0] != '.') {
        // 根据 -l 等选项显示文件的详细信息
        printf("%s\n", entry->d_name);
    }
}
closedir(dir);
if (closedir(dir) == -1) perror("closedir"); // 可以在这里进行进一步的错误处理，如记录错误日志、尝试恢复或采取其他适当的措施 return EXIT_FAILURE; }



    }