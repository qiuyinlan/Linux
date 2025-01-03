#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <bits/getopt_core.h>

// 文件信息结构体
typedef struct FileInfo {
    char name[256];
    struct stat stats;
} FileInfo;

// 参数全局变量
int show_hidden = 0, long_format = 0, recursive = 0, sort_time = 0, reverse = 0, show_inode = 0, show_size = 0;

// 函数声明
void list_directory(const char *path);
void display_file(const FileInfo *file);
void sort_files(FileInfo *files, int count);
void print_permissions(mode_t mode);

// 主函数：解析命令行参数
int main(int argc, char *argv[]) {
    // 解析参数
    int opt;
    while ((opt = getopt(argc, argv, "alRtirs")) != -1) {
        switch (opt) {
            case 'a': show_hidden = 1; break;
            case 'l': long_format = 1; break;
            case 'R': recursive = 1; break;
            case 't': sort_time = 1; break;
            case 'r': reverse = 1; break;
            case 'i': show_inode = 1; break;
            case 's': show_size = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-alRtirs] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // 处理目录参数
    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            list_directory(argv[i]);
            printf("\n");
        }
    } else {
        list_directory(".");
    }

    return 0;
}

// 列出目录内容
void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    FileInfo files[1024];
    int count = 0;

    // 遍历目录项
    while ((entry = readdir(dir)) != NULL) {
        // 跳过隐藏文件
        if (!show_hidden && entry->d_name[0] == '.')
            continue;

        // 获取文件信息
        FileInfo file;
        snprintf(file.name, sizeof(file.name), "%s/%s", path, entry->d_name);
        if (stat(file.name, &file.stats) == -1) {
            perror("stat");
            continue;
        }
        strcpy(file.name, entry->d_name); // 保存文件名
        files[count++] = file;
    }

    closedir(dir);

    // 排序
    sort_files(files, count);

    // 打印文件信息
    for (int i = 0; i < count; i++) {
        display_file(&files[i]);
    }

    // 递归处理子目录
    if (recursive) {
        for (int i = 0; i < count; i++) {
            if (S_ISDIR(files[i].stats.st_mode) &&
                strcmp(files[i].name, ".") != 0 &&
                strcmp(files[i].name, "..") != 0) {
                printf("\n%s/%s:\n", path, files[i].name);
                char subpath[1024];
                snprintf(subpath, sizeof(subpath), "%s/%s", path, files[i].name);
                list_directory(subpath);
            }
        }
    }
}

// 打印单个文件的信息
void display_file(const FileInfo *file) {
    if (show_inode) {
        printf("%lu ", file->stats.st_ino);
    }
    if (show_size) {
        printf("%ld ", file->stats.st_blocks);
    }
    if (long_format) {
        print_permissions(file->stats.st_mode);
        printf(" %ld", file->stats.st_nlink);
        printf(" %s", getpwuid(file->stats.st_uid)->pw_name);
        printf(" %s", getgrgid(file->stats.st_gid)->gr_name);
        printf(" %ld", file->stats.st_size);
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&file->stats.st_mtime));
        printf(" %s", timebuf);
    }
    printf(" %s\n", file->name);
}

// 排序文件
void sort_files(FileInfo *files, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int cmp;
            if (sort_time) {
                cmp = files[j].stats.st_mtime - files[j + 1].stats.st_mtime;
            } else {
                cmp = strcmp(files[j].name, files[j + 1].name);
            }
            if ((reverse && cmp < 0) || (!reverse && cmp > 0)) {
                FileInfo temp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = temp;
            }
        }
    }
}

// 打印权限
void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}
