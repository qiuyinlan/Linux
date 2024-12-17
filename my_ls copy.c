#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    // 定义目录指针
    DIR *dir;
    struct dirent *entry;

    // 如果未提供目录名，使用当前目录
    const char *path = (argc > 1) ? argv[1] : ".";

    // 打开目录
    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir"); // 输出错误信息
        return EXIT_FAILURE;
    }

    // 读取并打印目录内容（包括隐藏文件）
    printf("Contents of directory %s (including hidden files):\n", path);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // 关闭目录
    closedir(dir);

    return EXIT_SUCCESS;
}
